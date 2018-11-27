import os
import mysql.connector
import json

files = os.listdir("logs")

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()



mydb.commit()

for file in files:
	print file
	pid = file.split('.')[0].split('_')[1]
	print pid
	with open('logs/'+file) as f:
	    lines = f.readlines()
	    for line in lines:
	    	print line.strip() + ' ' + pid.strip()
	    	sql = "INSERT INTO strace (pid, val) VALUES (%s, %s)"
	    	val = (pid, line)
	    	mycursor.execute(sql, val)
	    	mydb.commit()
	    