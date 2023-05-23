#Pixel Wiring Map
#
#
#       18 17 16
#    19          15
#    20          14
#    21          13
#        3  2  1
#     4          12
#     5          11
#     6          10
#        7  8  9

import time
import board
import neopixel

# Define the GPIO pin number for Neopixel
pixel_pin = board.D18

# Define the number of LEDs in the LED strip
num_pixels = 86

# Create Neopixel object with the specified pin and number of pixels
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.2, auto_write=False)

# Define the LED segments for each digit
segment_map = {
    '0': [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21],
    '1': [10, 11, 12, 13, 14, 15],
    '2': [1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 14, 15, 16, 17, 18],
    '3': [1, 2, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18],
    '4': [1, 2, 3, 10, 11, 12, 13, 14, 15, 19, 20, 21],
    '5': [1, 2, 3, 7, 8, 9, 10, 11, 12, 16, 17, 18, 19, 20, 21],
    '6': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 17, 18, 19, 20, 21],
    '7': [10, 11, 12, 13, 14, 15, 16, 17, 18],
    '8': [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21],
    '9': [1, 2, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21]
}

def display_digit(digit, position):
    # Calculate the LED indices for the given digit and position in reverse order
    segment_leds = [(position * 21 + led -1) for led in segment_map[digit]]
    # Turn on the LEDs for the segment
    for led in segment_leds:
        pixels[led] = (0, 255, 0)

    # Update the Neopixel display
    pixels.show()

def clear_display():
    # Clear all LEDs on the strip
    pixels.fill((0, 0, 0))
    pixels.show()

def display_time():
    # Get the current time
    #current_time = time.strftime('%H%M')  #24 hour clock
    current_time = time.strftime('%I%M')  #12 hour clock

    # Display hours
    if current_time[0] != '0':
        display_digit(current_time[1], -1)  # print first digit of hour if not 0
    display_digit(current_time[1], -2)
    
    # Display dots (blinking once per second)
    dots_on = int(time.time()) % 2 == 0
    if dots_on:
        pixels[42] = (0, 255, 0)
        pixels[43] = (0, 255, 0)
    else:
        pixels[42] = (0, 0, 0)
        pixels[43] = (0, 0, 0)
    pixels.show()

    # Display minutes
    display_digit(current_time[2], 1)
    display_digit(current_time[3], 0)

# Clear the display initially
clear_display()

# Continuously update the time display
while True:
    display_time()
    time.sleep(1)
    clear_display()
