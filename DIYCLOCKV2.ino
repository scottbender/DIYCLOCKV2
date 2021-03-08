
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

char ssid[] = "yourssidhere";
char pass[] = "yourwifipasshere";

int hours =0;
int mins =0;

#define PIN            2
#define NUMPIXELS      100
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int localPort = 2390;

IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;

byte packetBuffer[ NTP_PACKET_SIZE];

WiFiUDP udp;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  pixels.begin();
}

void loop()
{
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); 
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    //Serial.println("no packet yet");
  }
  else {
    //Serial.print("packet received, length=");
    //Serial.println(cb);
    
    udp.read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;

    hours = (epoch  % 86400L) / 3600;
    if (hours<24)
    {
      hours++;  
    }
    if (hours==24)
    {
      hours=0;  
    }
    mins = (epoch  % 3600) / 60;
    
    Serial.print(hours); 
    Serial.print(':');
    Serial.println(mins); 

    drawdigit(0,255,0,0,hours/10);//Draw the first digit of the hour
    drawdigit(21,255,0,0,hours-((hours/10)*10));//Draw the second digit of the hour
    pixels.setPixelColor(42, pixels.Color(255,0,0));//Draw the two middle dots
    pixels.setPixelColor(43, pixels.Color(255,0,0));
    drawdigit(44,255,0,0,mins/10);//Draw the first digit of the minute
    drawdigit(65,255,0,0,mins-((mins/10)*10));//Draw the second digit of the minute

    delay(59500);//Request to NIST server should be separated by 60 seconds.

    





     
  }
}
 



void drawdigit(int offset, int r, int g, int b, int n)
{

  if (n==2 || n==3 || n== 4 || n==5 || n==6 || n==8 || n==9 )//MIDDLE
  {
    pixels.setPixelColor(0+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(1+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(2+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(0+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(1+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(2+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==1 || n== 2 || n==3 || n==4 || n==7 || n==8 || n==9)//TOP RIGHT
  {
    pixels.setPixelColor(3+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(4+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(5+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(3+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(4+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(5+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==2 || n== 3 || n==5 || n==6 || n==7 || n==8 || n==9)//TOP
  {
    pixels.setPixelColor(6+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(7+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(8+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(6+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(7+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(8+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==4 || n==5 || n== 6 || n==8 || n==9 )//TOP LEFT
  {
    pixels.setPixelColor(9+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(10+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(11+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(9+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(10+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(11+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==2 || n==6 || n== 8)//BOTTOM LEFT
  {
    pixels.setPixelColor(12+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(13+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(14+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(12+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(13+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(14+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==2 || n==3 || n== 5 || n==6 || n==8 || n==9)//BOTTOM
  {
    pixels.setPixelColor(15+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(16+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(17+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(15+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(16+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(17+offset, pixels.Color(0,0,0));
  }
  if (n==0 || n==1 || n==3 || n==4 || n==5 || n==6|| n==7 || n==8 || n==9)//BOTTOM RIGHT
  {
    pixels.setPixelColor(18+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(19+offset, pixels.Color(r,g,b));
    pixels.setPixelColor(20+offset, pixels.Color(r,g,b));
  }
  else
  {
    pixels.setPixelColor(18+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(19+offset, pixels.Color(0,0,0));
    pixels.setPixelColor(20+offset, pixels.Color(0,0,0));
  }
  pixels.show();
}



// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
