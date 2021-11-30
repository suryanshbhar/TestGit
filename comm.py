from firebase import firebase
import numpy as np
import time
from datetime import datetime


firebase = firebase.FirebaseApplication('https://capstone-7efdb-default-rtdb.firebaseio.com/', None)


###########################################

# Change this when needed. This is also the username you need to use when signing up/ logging in
username= 'suryansh'

while True:
    time.sleep(5)
    
    data=np.random.uniform(-20, 100, 4)
    for index,i in enumerate(data):
        if i<0:
            data[index]=0
            
    now = datetime.now()

    ctime = now.strftime("%H:%M:%S")
    data = {
        'sensor1': data[0],
        'sensor2': data[1],
        'sensor3': data[2],
        'sensor4': data[3],
        'time': ctime,
    }
    result = firebase.post(f'/{username}/',data)
    
