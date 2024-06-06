#!/usr/bin/python3
import cgi
import cgitb
import os

cgitb.enable()

#print("Content-Type: text/html\n")

# Parse query string
# query_string = os.environ.get('QUERY_STRING', '')
# form = cgi.parse_qs(query_string)
#username = form.get("username", [""])[0]

username = os.environ.get('username','')

print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>GET Test Result</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            text-align: center;
            background-color: #f0f0f0;
        }}
        .container {{
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            padding: 20px;
            width: 300px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>GET Test Result</h1>
        <p>Hello, {username} !</p>
    </div>
</body>
</html>
""")
