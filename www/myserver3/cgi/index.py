#!/usr/bin/env python3

import os
import cgi
import cgitb
import json
import sys

cgitb.enable()

# Set up absolute upload directory path
UPLOAD_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "uploads"))
os.makedirs(UPLOAD_DIR, exist_ok=True)

def list_files():
    try:
        files = os.listdir(UPLOAD_DIR)
        return {"status": "success", "files": files}
    except Exception as e:
        return {"status": "fail", "error": str(e)}

def save_file(form):
    if "file" not in form:
        return {"status": "fail", "error": "No file field in form"}

    file_item = form["file"]
    if file_item.filename:
        filename = os.path.basename(file_item.filename)
        filepath = os.path.join(UPLOAD_DIR, filename)

        try:
            with open(filepath, "wb") as f:
                f.write(file_item.file.read())
            return {"status": "success", "filename": filename}
        except Exception as e:
            return {"status": "fail", "error": str(e)}
    else:
        return {"status": "fail", "error": "No file selected"}

def delete_file(form):
    filename = form.getvalue("filename")
    if not filename:
        return {"status": "fail", "error": "No filename specified"}

    filepath = os.path.join(UPLOAD_DIR, os.path.basename(filename))
    if os.path.exists(filepath):
        try:
            os.remove(filepath)
            return {"status": "deleted", "filename": filename}
        except Exception as e:
            return {"status": "fail", "error": str(e)}
    else:
        return {"status": "fail", "error": "File not found"}

def main():
    print("Content-Type: application/json\n")

    form = cgi.FieldStorage()
    action = form.getvalue("action")

    if action == "upload":
        response = save_file(form)
    elif action == "list":
        response = list_files()
    elif action == "delete":
        response = delete_file(form)
    else:
        response = {"status": "fail", "error": "File not found"}

    print(json.dumps(response))

if __name__ == "__main__":
    main()
