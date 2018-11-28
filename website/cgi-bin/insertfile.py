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

file = form["pattern"].value
#file = 'rama'

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="iot"
)

mycursor = mydb.cursor()

mycursor.execute("insert into files(filename) values('"+str(file)+"')")

mydb.commit()

print json.dumps(200)






