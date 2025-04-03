<?php

echo "Content-Type: text/html";
echo "\n\n";

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PHP CGI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            text-align: center;
            padding: 20px;
        }
        .container {
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0px 0px 10px gray;
            display: inline-block;
        }
        h1 {
            color: #4CAF50;
        }
        p {
            color: #333;
        }
        .button {
            margin-top: 20px;
        }
        .button a {
            text-decoration: none;
            color: white;
            background-color: #4CAF50;
            padding: 10px 20px;
            border-radius: 5px;
            font-size: 16px;
        }
        .button a:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>PHP CGI</h1>
        <p>This page is generated using a PHP CGI script.</p>
        <p><strong>What is CGI?</strong> Common Gateway Interface (CGI) is a protocol that allows web servers to execute scripts and dynamically generate web content.</p>
        <p><strong>Current CGI Script:</strong> PHP is handling this request!</p>
        <p><strong>Server Time:</strong> <?php echo date('Y-m-d H:i:s'); ?></p>
        <div class="button">
            <a href="http://localhost:8080/cgi-bin/hello.py">Go to Python CGI Script</a>
        </div>
    </div>
</body>
</html>