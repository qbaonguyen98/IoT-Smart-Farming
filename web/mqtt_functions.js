var mqtt;
var reconnectTimeout = 2000;
var host="192.168.1.101"; 
var port=9001;      // websocket port: make sure mosquitto is configured with websocket

function onConnect() {
    console.log("Connected ");
}

function MQTTconnect() {
    console.log("connecting to "+ host +" "+ port);
    mqtt = new Paho.MQTT.Client(host,port,"clientjs");
    var options = {
        timeout: 3,
        onSuccess: onConnect,  
    };
    mqtt.connect(options); //connect
}

function lighton() {
    message = new Paho.MQTT.Message("{\"Light\":\"ON\",\"Pump\":\"UNCHANGED\"}");
    message.destinationName = "Relays";     //topic
    mqtt.send(message);
    console.log("Light ON");
}

function lightoff() {
    message = new Paho.MQTT.Message("{\"Light\":\"OFF\",\"Pump\":\"UNCHANGED\"}");
    message.destinationName = "Relays";     //topic
    mqtt.send(message);
    console.log("Light OFF");
}

function pumpon() {
    message = new Paho.MQTT.Message("{\"Light\":\"UNCHANGED\",\"Pump\":\"ON\"}");
    message.destinationName = "Relays";     //topic
    mqtt.send(message);
    console.log("Pump ON");
}

function pumpoff() {
    message = new Paho.MQTT.Message("{\"Light\":\"UNCHANGED\",\"Pump\":\"OFF\"}");
    message.destinationName = "Relays";     //topic
    mqtt.send(message);
    console.log("Pump OFF");
}