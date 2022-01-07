#!/usr/bin/python3

import RPi.GPIO as GPIO
import sys
from time import sleep
from RpiMotorLib import RpiMotorLib

# User inputs desired wait times as arguments
# python3 sampler.py 1 2 3 5 10 ... up to 16 individual arguments/slots.
# Program waits for set amount of MINUTES, samples and moves to next spot.

schedu = sys.argv[1:]
if len(schedu) >= 17:
    sys.exit("Ran out of slots")
GPIO_pins = (14,15,18)
direction = 20
step = 21
edge_p = 4 # Edge switch, zeroes table
enab = 3 # enable pin on A4988

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(edge_p, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(enab, GPIO.OUT)
ajo = RpiMotorLib.A4988Nema(direction, step, GPIO_pins, "A4988")

# Offsets for slots, if not perfectly symmetrical
slot_diff = [200,200,200,200, 
            190,200,200,220, 
            180,220,220,180,
            200,200,180,180, 
            ]
init_offset = 110

def init():
    ''' Rotates sample table until it hits end stop, resetting position '''
    GPIO.output(enab, 0) #0 run, 1 stop.
    while GPIO.input(edge_p) == GPIO.LOW:
        ajo.motor_go(False, "1/16", 10, 0.001, False, 0.05)
    ajo.motor_go(True, "1/16", init_offset, 0.003, False, 0.05)
    GPIO.output(enab,1) 
    print(f"Init complete. Program contains {len(schedu)} parts")

def grabSample():
    print("Sampling ...") # Servo to sampling location
    sleep(1) # userdefined, based on flowrate and desider volume
    print("Sampling complete") # Servo to bypass location
    sleep(1)

def main():
    print("main")
    for i in enumerate(schedu):
        print(f"Mainloop {i} : {slot_diff[i[0]]}")
        sleep(int(i[1]*1)) # seconds or mins, change here for easy debug
        grabSample()
        # next slot
        GPIO.output(enab,0) 
        if i[0]+1!= len(schedu):
            ajo.motor_go(True, "1/16", slot_diff[i[0]], 0.005, False, 0.05)
        GPIO.output(enab,1) 

if __name__ == "__main__":
    init()
    main()
