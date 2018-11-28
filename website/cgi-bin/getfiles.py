#!/usr/bin/env python
import mysql.connector
import json
from collections import Counter
import cgi


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

mycursor.execute("select filename from files")
filelist = mycursor.fetchall()
res = []
for file in filelist:
	res.append(file[0])

print json.dumps(res)







