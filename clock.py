#!/usr/bin/python3

# Pixel Wiring Map (From front of clock)
#
#
#    83 82 81       62 61 60            39 38 37       18 17 16
# 84         80  63          59      40         36  19          15
# 85         79  64          58  43  41         35  20          14
# 86         78  65          57      42         34  21          13
#   68 67 66        47 46 45            24 23 22        3  2  1
# 69         77  48          56      25         33   4          12
# 70         76  49          55  44  26         32   5          11
# 71         75  50          54      27         31   6          10
#    72 73 74       51 52 53            28 29 30        7  8  9

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
    for led in range(21):   #clear out pixels in case of digit change
        pixels[led] = (0, 0, 0)
    for led in segment_leds:
        pixels[led] = (0, 255, 0)

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

    # Display minutes
    display_digit(current_time[2], 1)
    display_digit(current_time[3], 0)
    pixels.show()

# Clear the display initially
clear_display()

# Continuously update the time display
while True:
    display_time()
    time.sleep(1)
