import mysql.connector
import datetime
from random import *

currentDT = datetime.datetime.now()
date = currentDT.strftime("%Y-%m-%d")
time = currentDT.strftime("%H:%M:%S")
print(date)
print(time)

moisture = randint(35,70)
brightness = randint(200,500)
temperature = randint(20,40)
humidity = randint(40,80)

print(moisture)       # for test

mydb = mysql.connector.connect(
  host="localhost",
  user="reusnguyen1199",
  passwd="1199",
  database="IoT"
)

mycursor = mydb.cursor()

sql =   "INSERT INTO Sensors (Date, Time, Moisture, Brightness, Temperature, Humidity) \
        VALUES (%s,%s,%s,%s,%s,%s)"
val = (date, time, moisture, brightness, temperature, humidity)
mycursor.execute(sql, val)

mydb.commit()

print(mycursor.rowcount, "sensors data inserted.")
##########################################################################################
sql =   "INSERT INTO Relays (Date, Time, Light, Pump) \
        VALUES (%s,%s,%s,%s)"
val = (date, time, "ON", "OFF")
mycursor.execute(sql, val)

mydb.commit()

print(mycursor.rowcount, "relays data inserted.")

mydb.close()
mycursor.close()