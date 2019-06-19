############################################################
###########     IoT Smart Farming Project     ##############
############################################################

#   Notes: - This project is designed to work in LAN, the MQTT broker and Web server is installed on Ubuntu 16.04
#   	   - You have to purchase a Web hosting, setup DNS and modify MQTT Server address to use it in the Internet

#   Follow these steps to make it work

Step 1: Flash code for NodeMCU by using "esp.c" file (you have to include libraries in Arduino libraries folder)

Step 2: Install these stuffs on a computer used as Server

    a)  Install Mosquitto and configure it to make sure the websocket is worked by following this link:
            https://obrienlabs.net/how-to-setup-your-own-mqtt-broker/

            (I ignored the user-password setup process)

    b)  Install "paho-mqtt-client" python library and python 2.7 (if not available)

    c)  Install MySQL, PHP, Apache2 and put the files and folders in "Web" folder to your Apache/www/Public HTML folder

Step 3: Run "create_db.py" and "create_tables.py" to create database and add tables
        Run "listen_mqtt.py" to get MQTT messages and automatically insert to database.