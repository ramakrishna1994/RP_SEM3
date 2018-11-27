#!/usr/bin/env python
import mysql.connector
import json


print """\
<html>
<body>
 &nbsp;
</body>
</html>
"""
mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()

mycursor.execute("SELECT pid,count(*) as cnt FROM strace group by pid order by cnt desc limit 5")

myresult = mycursor.fetchall()

res = []
for x in myresult:
	mm = {}
	mm['pid'] = x[0]
	mm['cnt'] = x[1]
	res.append(mm)

print json.dumps(res)


