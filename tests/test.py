import requests
import pytest
import unittest

BLU = "\033[94m"
GRN = "\033[92m"
RED = "\033[91m"
RES = "\033[0m"

class I_url():
    def __init__(self, url, expected):
        self.url = url
        self.expected = expected

valid_cgi = [
    I_url("http://localhost:8080/cgi-bin/hello.py", 200),
    I_url("http://localhost:8080/cgi-bin/hello.sh", 200),
    I_url("http://localhost:8080/cgi-bin/hello.php", 200),
]

invalid_cgi = [
    I_url("http://localhost:8080/cgi-bin/hello.pl", 501),
    I_url("http://localhost:8080/cgi-bin/", 403),
    I_url("http://localhost:8080/cgi-bin/hello", 404),
    I_url("http://localhost:8080/cgi-bin/hello.py?param=invalid", 400),
    I_url("http://localhost:8080/cgi-bin/empty_body.py", 500),
    I_url("http://localhost:8080/cgi-bin/empty_headers.py", 500),
    I_url("http://localhost:8080/cgi-bin/empty_content_type.py", 500),
    I_url("http://localhost:8080/cgi-bin/no_permissions.py", 500),
]

def test_url(url: list[I_url]):
    for i in url:
        response = requests.get(i.url)
        if response.status_code == i.expected:
            print(f"{GRN}OK {i.url} {response.status_code}{RES}")
        else:
            print(f"{RED}FAIL {i.url} {response.status_code}{RES}")
    
if __name__ == "__main__":
    print(f"{BLU}Testing valid CGI scripts...{RES}")
    test_url(valid_cgi)
    print(f"{BLU}Testing invalid CGI scripts...{RES}")
    test_url(invalid_cgi)