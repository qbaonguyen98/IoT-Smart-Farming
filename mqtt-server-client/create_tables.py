import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="reusnguyen1199",
  passwd="1199",
  database="IoT"
)

mycursor = mydb.cursor()

mycursor.execute("CREATE TABLE Sensors (\
                            id INT NOT NULL AUTO_INCREMENT,\
                            Date VARCHAR(50) NOT NULL, \
                            Time TIME NOT NULL, \
                            Moisture FLOAT(5,2) NOT NULL, \
                            Brightness FLOAT(5,2) NOT NULL, \
                            Temperature FLOAT(4,2) NOT NULL, \
                            Humidity FLOAT(5,2) NOT NULL, \
                            PRIMARY KEY (id))")

mycursor.execute("CREATE TABLE Relays (\
                            id INT NOT NULL AUTO_INCREMENT,\
                            Date VARCHAR(50) NOT NULL, \
                            Time TIME NOT NULL, \
                            Light VARCHAR(19) NOT NULL, \
                            Pump VARCHAR(19) NOT NULL, \
                            PRIMARY KEY (id))")

mycursor.execute("SHOW TABLES")
for x in mycursor:
  print(x)

mydb.close()
mycursor.close()