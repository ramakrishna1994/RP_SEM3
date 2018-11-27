#!/usr/bin/env python
import mysql.connector
import json
from collections import Counter

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

mycursor.execute("SELECT * FROM top5pidswithtop5systemcalls")

myresult = mycursor.fetchall()

res = []

for x in myresult:
	mm = []
	mm.append(x[0])
	mm.append(x[1])
	mm.append(x[2])
	res.append(mm)


print json.dumps(res)


