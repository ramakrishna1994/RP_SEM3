#!/usr/bin/env python



import mysql.connector
import json

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()

mycursor.execute("SELECT * FROM running")

myresult = mycursor.fetchall()


print """\
<html>
<body>
 &nbsp;
</body>
</html>
"""

pidlist = []
for x in myresult:
  #print(x[0])
  pidlist.append(x[0])


print json.dumps(pidlist);