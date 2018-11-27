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

mycursor.execute("truncate table top5pidswithtop5systemcalls")
mydb.commit()

mycursor.execute("SELECT pid,count(*) as cnt FROM strace group by pid order by cnt desc limit 5")

myresult = mycursor.fetchall()

for x in myresult:
	mycursor.execute("SELECT val FROM strace where pid="+x[0])
	result = mycursor.fetchall()
	calls = []
	for y in result:
		call = y[0].split('(')[0]
		calls.append(call)
	res = Counter(calls).most_common(1)
	mycursor.execute('insert into top5pidswithtop5systemcalls values('+str(x[0])+',"'+str(res[0][0])+'",'+str(res[0][1])+');')
	mydb.commit()





