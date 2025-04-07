#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()

form = cgi.FieldStorage()

print("Content-Type: text/html\n")

print(f"""
<!DOCTYPE html>
<html>
<head>
    <title>Hello CGI</title>
</head>
<body>
    <h1>Hello from CGI!</h1>
    <h2>Received Parameters:</h2>
    <ul>
""")

# Iterate over all fields and display them
for key in form.keys():
    value = form.getvalue(key, 'Not provided')
    print(f"        <li>{key}: {value}</li>")

print("""
    </ul>
</body>
</html>
""")