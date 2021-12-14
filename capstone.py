import time
from w1thermsensor import W1ThermSensor
from firebase import firebase
import numpy as np
from datetime import datetime
import board
import busio
import adafruit_ads1x15.ads1015 as ADS
from adafruit_ads1x15.analog_in import AnalogIn
from gpiozero import Buzzer
from time import sleep


buzzer = Buzzer(17)


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

print("Initializing Sensors......")


while True:
    time.sleep(5)
    
    temf=0
    turbf=0
    tdsf=0
    phf=0
    
    print("Gathering Sensor Data")
    try:
        temperature = sensor.get_temperature()
    except:
        temperature="error"
        temf=1
    print("The temperature is %s celsius" % temperature)
    try:
        ph = 3.28*chan.voltage+1.28
    except:
        ph="error"
        phf=1
    print("The pH is %s" % ph)
    
    try:
        ntu = (-118.1*man.voltage) + 461.15
    except:
        ntu="error"
        turbf=1
    print("The ntu units are %s" % ntu)
    
    try:
        v = ban.voltage
        tds=(133.42*v*v*v -255.86*v*v+857.39*v)*0.5
    except:
        tds="error"
        tdsf=1
    print("The tds units are %s" % tds)

    flag = 0
    
    

    if (temf!=1 and (temperature > 40 or temperature< 5) ) :
        print("TEMPERATURE ALERT !")
        flag=1
    
    if (phf!=1 and (ph<4 or ph>9)):
        print("pH ALERT !")
        flag=1
    
    if (turbf!=1 and (ntu>350 )):
        print("TURBIDITY ALERT !")
        flag=1
    
    if (tdsf!=1 and (tds>380)):
        print("TDS sensor ALERT !")
        flag=1
        
    if temf or phf or turbf or tdsf:
        print('Errors detected in sensors !')
    

    if (flag == 1):
        buzzer.on()
        time.sleep(5)
        print('ML prediction 0')
        print('Treatment Motor ON')
    
    else:
        buzzer.off()
        print("Water Quality Appropiate")
        time.sleep(2)
        print('ML prediction 1')
        print('Treatment Motor OFF')
    



    print('Sending data....')
    
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
    time.sleep(2)
    print('Data sent successfully')
    print('-------------------------------------------------------------------------------------------')
