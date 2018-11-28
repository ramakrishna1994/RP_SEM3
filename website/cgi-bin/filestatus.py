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
	mycursor.execute("SELECT pid,count(*) FROM strace where val like '%"+str(file[0])+"%' group by pid;")	
	myresult = mycursor.fetchall()
	for x in myresult:
		mm = {}
		mm["count"] = x[1]
		mm["pid"] = x[0]
		mm["file"] = file;
		res.append(mm)
	

print json.dumps(res)





