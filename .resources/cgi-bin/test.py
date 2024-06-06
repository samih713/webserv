#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()

print("Content-Type: text/html")
print()

form = cgi.FieldStorage()

if form:
    data = form.getvalue('data')
    print("<h1>Received Data</h1>")
    print(f"<p>{data}</p>")
else:
    print("<h1>No data received</h1>")