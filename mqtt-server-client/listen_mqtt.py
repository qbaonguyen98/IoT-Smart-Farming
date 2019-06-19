import paho.mqtt.client as mqtt
from store_data import data_handler

# MQTT Settings 
MQTT_Broker = "localhost"
MQTT_Port = 1883
Keep_Alive_Interval = 45
MQTT_Topic1 = "Sensors"
MQTT_Topic2 = "Relays"

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe(MQTT_Topic1)
  client.subscribe(MQTT_Topic2)

def on_message(client, userdata, msg):
    print("MQTT Data Received...")
    print("MQTT Topic: " + msg.topic)  
    print("Data: " + str(msg.payload))
    data_handler(msg.topic, msg.payload)

    
client = mqtt.Client()
client.connect(MQTT_Broker,MQTT_Port,Keep_Alive_Interval)

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()


# NOTES: ******* type "python listen_mqtt.py" on terminal to run ********