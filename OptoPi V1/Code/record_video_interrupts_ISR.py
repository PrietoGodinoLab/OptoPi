import tkinter as tk
import picamera
import time
import datetime as dt
import serial
import RPi.GPIO as GPIO
from time import sleep

interrupt_pin = 21
#GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(interrupt_pin, GPIO.OUT, initial=GPIO.LOW)


USB_PORT = "/dev/ttyACM0"
#USB_PORT = "/dev/ttyACM1"
parent_directory = '/home/pi/Documents/dmel'
date='2021_06_11'
exp='1DR_test'
lighting = 'red'
lighting_paradigm = 'OFF_ON'
gal4 = 'NA'
uas = 'NA'
experiment_name = parent_directory + '/' + date + '_' + gal4 + '_' + uas + '_' + lighting + '_' + lighting_paradigm + '_'+ exp


experiment_duration=60

def ON_ISR(stim_dur):
    #camera.annotate_text = str((dt.datetime.now()-start).total_seconds()) + '_ON'
    camera.annotate_text = 'ON'
    file.write('\n'+ str((dt.datetime.now()-start).total_seconds()) + '_ON')
    GPIO.output(interrupt_pin, GPIO.HIGH)
    sleep(stim_dur)
    

def OFF_ISR(stim_dur):
    camera.annotate_text = 'OFF'
    #camera.annotate_text = str((dt.datetime.now()-start).total_seconds()) + '_OFF'
    file.write('\n'+ str((dt.datetime.now()-start).total_seconds()) + '_OFF')
    GPIO.output(interrupt_pin, GPIO.LOW)
    sleep(stim_dur)

try:
   ser = serial.Serial(USB_PORT, 9600, timeout=1)
   ser.flush()
except:
   print("ERROR - Could not open USB serial port.  Please check your port name and permissions.")
   print("Exiting program.")
   exit()


with picamera.PiCamera() as camera:
        camera.resolution = (1000, 1200)
        camera.color_effects=(128,128)
        camera.framerate = 10
        camera.annotate_background = picamera.Color('black')
        camera.start_preview(alpha=250)
        file = open(experiment_name + "_log.csv","a")
        input('Press ENTER to start recording')
        start = dt.datetime.now()
        camera.start_recording(experiment_name + ".h264")
        while not (dt.datetime.now() - start).total_seconds() > experiment_duration:
            ON_ISR(0.1) #set whatever duration the light should be on in seconds
            OFF_ISR(0.1) #set whatever duration the light should be off in seconds

        camera.stop_recording()
        file.close()
        ser.flush()
ser.flush()
