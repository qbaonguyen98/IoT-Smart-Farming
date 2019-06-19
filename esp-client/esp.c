#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
//#include <NTPClient.h>      //Download NTPClient-Master on Github
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Ticker.h>

//---------------------------------------------- Initialize sensor pins -------------------------------------------
#define DHTTYPE DHT11 
#define DHTPIN D5               // DHT sensor pin

#define moisture_pin  A0        // moisture sensor pin
const int dry_value = 850;
const int wet_value = 470;
float moisture_value;

#define light_pin  A0        // light sensor pin
float light_value = 0;

#define set D0                  // to control Analog MUX

#define BTN1 D4                 // button
#define BTN2 D3                 // pins
bool BTN1_Status=0; 
bool BTN2_Status=0;

#define RelayControl_1 D7       // relay
#define RelayControl_2 D8       // pins

const char subscribe_topic[] = "Relays";

Ticker MQTT_Service;
volatile int TimerCounter=0;
volatile bool FlagMQTT=1;

//---------------------------------------------- Configure Network ------------------------------------------------
const char* ssid =         "tam";
const char* password =    "12345678";
const char* mqtt_server = "192.168.1.101"; 
const uint16_t mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

//---------------------------------------------- Configure LCD ----------------------------------------------------
int lcdColumns = 16;            // LCD columns
int lcdRows = 2;                // LCD rows

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
DHTesp dht;

//==========================================================================================
//===                                   Connect to WIFI                                  ===
//==========================================================================================
void setup_wifi()      
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//==========================================================================================
//===                               Buttons Interrupt Handler                            ===
//==========================================================================================
void handlerInterrupt()        
{
  if(digitalRead(BTN1)==0)
  {
    BTN1_Status = !BTN1_Status;
    digitalWrite(RelayControl_1,BTN1_Status);
  }
  if(digitalRead(BTN2)==0)
  {
    BTN2_Status = !BTN2_Status;
    digitalWrite(RelayControl_2,BTN2_Status);
  }
  
  if(BTN1_Status==1 && BTN2_Status==1){
    client.publish("Relays","{\"Light\":\"ON\",\"Pump\":\"ON\"}");
  }
  else if(BTN1_Status==1 && BTN2_Status==0){
    client.publish("Relays","{\"Light\":\"ON\",\"Pump\":\"OFF\"}");
  }
  else if(BTN1_Status==0 && BTN2_Status==1){
    client.publish("Relays","{\"Light\":\"OFF\",\"Pump\":\"ON\"}");
  }
  else if(BTN1_Status==0 && BTN2_Status==0){
    client.publish("Relays","{\"Light\":\"OFF\",\"Pump\":\"OFF\"}");
  }
  
}
//==========================================================================================
//===                                  Connect to MQTT broker                            ===
//==========================================================================================
void reconnect()               
{
  // wait until connected
  while (!client.connected()) {
    Serial.println(""); 
    Serial.print("\n\rAttempting MQTT connection... ");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected");
      client.publish("event", "connection established");
      client.subscribe(subscribe_topic);
    } 
    else {
      Serial.print("\n\rFailed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
//==========================================================================================
//===                               MQTT Message Handler                                 ===
//==========================================================================================
void callback(char* topic, byte* message, unsigned int msg_length)        
{                                                                     
  // print incoming message
  Serial.println("\n\r");
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;   // to store message in char datatype
  for (int i = 0; i < msg_length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Changes the output state according to the message
  if (String(topic) == subscribe_topic) {
    if(messageTemp == "{\"Light\":\"ON\",\"Pump\":\"UNCHANGED\"}"){
      Serial.println("Light: ON");
      digitalWrite(RelayControl_1,1);
      BTN1_Status = 1;
    }
    else if(messageTemp == "{\"Light\":\"OFF\",\"Pump\":\"UNCHANGED\"}"){
      Serial.println("Light: OFF");
      digitalWrite(RelayControl_1,0);
      BTN1_Status = 0;
    }
    else if(messageTemp == "{\"Light\":\"UNCHANGED\",\"Pump\":\"ON\"}"){
      Serial.println("Pump: ON");
      digitalWrite(RelayControl_2,1);
      BTN2_Status = 1;
    }
    else if(messageTemp == "{\"Light\":\"UNCHANGED\",\"Pump\":\"OFF\"}"){
      Serial.println("Pump: OFF");
      digitalWrite(RelayControl_2,0);
      BTN2_Status = 0;
    }
    else{
      Serial.println("Invalid message");
    }
  }
  else{
    Serial.println("Invalid Topic");
  }
  Serial.println();
}
//==========================================================================================
//===                             Setup() and Loop() Functions                           ===
//==========================================================================================
void setup()
{ 
  lcd.begin();                 
  lcd.backlight();              // turn on LCD backlight 
  Serial.begin(115200);
  setup_wifi();
  lcd.print("DAMH ");
  Serial.println("********* DAMH *********");
  delay(1000);  
  pinMode(set, OUTPUT);
  dht.setup(D5, DHTesp::DHT11); // Connect DHT sensor to GPIO 17
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);
  pinMode(RelayControl_1,OUTPUT);
  pinMode(RelayControl_2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BTN1),handlerInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2),handlerInterrupt, FALLING);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //timeClient.begin();
  
  MQTT_Service.attach(0.5, MQTT_TimerInt); //handle call-back function after every 0.5 second 
}

void MQTT_TimerInt(void)
{
  TimerCounter++;
  if (TimerCounter == 10){         //10 x 0.5s = 5s
    FlagMQTT=1;
    TimerCounter=0;
  }
}

void loop()
{  
  //==========================================================================================
  //===                                   Read Sensors                                     ===
  //========================================================================================== 
  digitalWrite(set, LOW);      // Init Analog MUX pin to read moisture sensor
  delay(100);
  moisture_value = analogRead(moisture_pin);
  moisture_value = map(moisture_value,dry_value,wet_value,0,100);

  digitalWrite(D0, HIGH);      // Then toggle to read light sensor
  delay(100);
  light_value= analogRead(light_pin);

  // Read DHT sensor
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  //==========================================================================================
  //===                             Display Sensors Data on LCD                            ===
  //==========================================================================================
  lcd.setCursor(0, 0); 
  lcd.print("Moisture: ");
  lcd.print(moisture_value);
  lcd.print(" %");
  lcd.setCursor(0, 1); 
  lcd.print("Light: ");
  lcd.print(light_value); 
  lcd.print(" nit");
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0); 
  lcd.print("Air_Temp: ");
  lcd.print(temperature);
  lcd.setCursor(0, 1); 
  lcd.print("Air_Humidy: ");
  lcd.print(humidity);
  delay(1000);

  //==========================================================================================
  //===                           Display Relay Status on LCD                              ===
  //==========================================================================================
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Relay 1: ");
  if(BTN1_Status ==0) lcd.print("OFF ");
  else lcd.print("ON");
  lcd.setCursor(0, 1);
  lcd.print("Relay 2: ");
  if(BTN2_Status ==0) lcd.print("OFF ");
  else lcd.print("ON");
  delay(1000);
  lcd.clear();

              // just for test
              //  moisture_value = random(40,70);
              //  light_value = random(200,500);
              //  temperature = random(20,50);
              //  humidity = random(40,80);

  //==========================================================================================
  //===                                Publish MQTT message                                ===
  //==========================================================================================
  if (!client.connected()) {
    reconnect();
  }

  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> data;
  
  data["Moisture"] = moisture_value;
  data["Brightness"] = light_value;
  data["Temperature"] = temperature;
  data["Humidity"] = humidity;
  
  char Jsondata[256];
  serializeJson(data, Jsondata);

  if (FlagMQTT){
    Serial.println("\r\n");
    Serial.println(Jsondata);

    if (client.publish("Sensors", Jsondata) == true) {
      Serial.println("Success sending Sensors data");
    } else {
      Serial.println("Error sending data");
    }

    FlagMQTT=0;
  }
  client.loop();
  Serial.println("");
}