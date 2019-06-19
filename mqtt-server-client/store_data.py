import mysql.connector
import datetime
import json

################################################################
###                Insert sensor data to MySQL               ###
################################################################
def sensors_data_handler(jsonData):
    mydb = mysql.connector.connect(
        host='localhost',
        user="reusnguyen1199",
        passwd="1199",
        database="IoT"
    )
    mycursor = mydb.cursor()

	#Parse Data 
    json_Dict = json.loads(jsonData)                    # parse jsonData
    Moisture = json_Dict["Moisture"]
    Brightness = json_Dict["Brightness"]
    Temperature = json_Dict["Temperature"]
    Humidity = json_Dict["Humidity"]

    currentDT = datetime.datetime.now()
    date = currentDT.strftime("%Y-%m-%d")
    time = currentDT.strftime("%H:%M:%S")

    sql = "INSERT INTO Sensors (Date, Time, Moisture, Brightness, Temperature, Humidity) VALUES (%s,%s,%s,%s,%s,%s)"
    val = (date, time, Moisture, Brightness, Temperature, Humidity)
    mycursor.execute(sql, val)
    mydb.commit()
    print(mycursor.rowcount, "sensors data inserted.")

################################################################
###                Insert Relay data to MySQL               ###
################################################################
def relays_data_handler(jsonData):
    mydb = mysql.connector.connect(
        host='localhost',
        user="reusnguyen1199",
        passwd="1199",
        database="IoT"
    )
    mycursor = mydb.cursor()

	#Parse Data 
    json_Dict = json.loads(jsonData)                    # parse jsonData
    light = json_Dict["Light"]
    pump = json_Dict["Pump"]

    currentDT = datetime.datetime.now()
    date = currentDT.strftime("%Y-%m-%d")
    time = currentDT.strftime("%H:%M:%S")
    
    sql = "INSERT INTO Relays (Date, Time, Light, Pump) VALUES (%s, %s, %s, %s)"
    val = (date, time, light, pump)
    mycursor.execute(sql, val)
    mydb.commit()
    print(mycursor.rowcount, "relays data inserted.")

#===============================================================
# Master Function to Select DB Funtion based on MQTT Topic

def data_handler(Topic,jsonData):
    print("Inserting data into database.....")
    if (Topic == "Sensors"):
        sensors_data_handler(jsonData)
    elif (Topic == "Relays"):
        relays_data_handler(jsonData)

#===============================================================