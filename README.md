# Pastebin
## Use case

 - **User** enters a block of text and gets a randomly generated link
```
$ curl -X POST --data '{ \"expiration_length_in_minutes\": 60,
 \"paste_contents\": \"Hello World!\" }' https://localhost:8080/paste
```
Response
```
{ "shortlink": "foobar" }
```
 - **User**  enters a paste's url and views the contents
```
$ curl https://pastebin.com/api/v1/paste?shortlink=foobar
```
```
{
    "paste_contents": "Hello World"
    "created_at": "YYYY-MM-DD HH:MM:SS"
    "expiration_length_in_minutes": "60"
}
```

## Design
![system design](res/system.png)

## Rationale

We could use a relational database as a large harsh table, mapping the generated url to a file server.
Instead of managing a file server ourselves, we could use a free data store service such as MongoDB cloud.

### User case 1
- The Client sends a create paste request to the Web Server, running as a reverse proxy
- The Web Server forwards the request to the Write API server
- The Write API server does the following:
  - Generates a unique url
  - Saves to the SQL Database pastes table
  - Saves the paste data to the Object Store
  - Returns the url

### User case 2
- The Client sends a get paste request to the Web Server
- The Web Server forwards the request to the Read API server
- The Read API server does the following:
  - Checks the SQL Database for the generated url
    - If the url is in the SQL Database, fetch the paste contents from the Object Store
    - Else, return an error message for the user

## Compile
### third party

```
vcpkg install sqlpp11-connector-sqlite3:x64-windows mongo-cxx-driver:x64-windows nlohmann-json:x64-windows boost-beast:x64-windows abseil:x64-windows
```

### compiler
VS2019 or above(C++17)

## Extension
- Load balancer for Web server
- Memory cache for Read API
- Master-Slave Replicas for write&Read API