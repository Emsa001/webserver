#!/usr/bin/env python3
import sys

print("Content-Type: text/plain\n")
data = sys.stdin.read()
print(f"POST data: {data}")