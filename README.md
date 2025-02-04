hello from beqa branch


### GET
```Bash
curl http://localhost:8080
```

### POST (file upload)
```Bash
curl -X POST -H "Content-Length: 13" -d "Hello, World" http://localhost:8080/upload
```

### DELETE (remove file)
```Bash
curl -X DELETE http://localhost:8080/uploads/file.txt
```

### Test 
```Bash
siege -c255 -t100 -r22 http://localhost:8080
```

