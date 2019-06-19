import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="reusnguyen1199",
  passwd="1199"
)
print(mydb)

# Create database
mycursor = mydb.cursor()
mycursor.execute("CREATE DATABASE IoT")

# Check 
mycursor.execute("SHOW DATABASES")
for x in mycursor:
  print(x)

mydb.close()
mycursor.close()