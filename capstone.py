import time
from w1thermsensor import W1ThermSensor
from firebase import firebase
import numpy as np
from datetime import datetime
import board
import busio
import adafruit_ads1x15.ads1015 as ADS
from adafruit_ads1x15.analog_in import AnalogIn


firebase = firebase.FirebaseApplication('https://capstone-7efdb-default-rtdb.firebaseio.com/', None)
username= 'suryansh'

#temperature
sensor = W1ThermSensor()

#pH sensor
i2c = busio.I2C(board.SCL,board.SDA)

ads = ADS.ADS1015(i2c)
ads.gain = 1

chan = AnalogIn(ads,ADS.P0)
man = AnalogIn(ads,ADS.P1)
ban = AnalogIn(ads,ADS.P2)


while True:
    time.sleep(5)
    try:
        temperature = sensor.get_temperature()
    except:
        temperature="error"
    print("The temperature is %s celsius" % temperature)
    try:
        ph = 3.28*chan.voltage+1.28
    except:
        ph="error"
    print("The pH is %s" % ph)
    
    try:
        ntu = (-118.4*man.voltage) + 461.15
    except:
        ntu="error"
    print("The ntu units are %s" % ntu)
    
    try:
        v = ban.voltage
        tds=(133.42*v*v*v -255.86*v*v+857.39*v)*0.5
    except:
        tds="error"
    print("The ntu units are %s" % tds)
    
    #data transmission
    now = datetime.now()
    ctime = now.strftime("%H:%M:%S")
    data = {
        'sensor1': temperature,
        'sensor2': ph,
        'sensor3': ntu,
        'sensor4': tds,
        'time': ctime,
    }
    result = firebase.post(f'/{username}/',data)
