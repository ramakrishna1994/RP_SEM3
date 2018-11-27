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


form = cgi.FieldStorage()

pid = form["pid"].value
#pid = 2147

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()

mycursor.execute("SELECT val FROM strace where pid="+str(pid))

myresult = mycursor.fetchall()

calls = []

for x in myresult:
	call = x[0].split('(')[0]
	calls.append(call)

res = Counter(calls).most_common(10)

print json.dumps(res)





