#!/usr/bin/env python
import mysql.connector
import json
from collections import Counter


mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()

mycursor.execute("SELECT val FROM strace")

myresult = mycursor.fetchall()

calls = []

for x in myresult:
	call = x[0].split('(')[0]
	calls.append(call)

res = Counter(calls).most_common(5)

mycursor.execute("truncate table systemcalls")
mydb.commit()

for pid,cnt in res:
	sql = "INSERT INTO systemcalls(scall,cnt) VALUES (%s, %s)"
	val = (pid, cnt)
	mycursor.execute(sql, val)
	mydb.commit()




