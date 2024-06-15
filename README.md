# UDP_ClientServer
Simple UDP Client Server project

# Server

Build the server :

```
cd Server
make
```

Launch the server : 

```
./udp_server
```

You will see the server launched successfully with the message :
```
Server listening on port 8080
```

# Client

Build the client :

```
cd QT_Client
qmake
make
```

Launch a client : 

On Linux :
```
./udp_client
```

On MacOS :
```
./udp_client.app/Contents/MacOS/udp_client
```

<img width="396" alt="Client" src="https://github.com/DigitalBox98/UDP_ClientServer/assets/57635141/2be2a0db-bbac-495a-9966-3b559f513d5e">

Select the server address, type your message and then send : 

<img width="399" alt="Hello" src="https://github.com/DigitalBox98/UDP_ClientServer/assets/57635141/a6dcfb34-dd47-4088-9fc6-f4169aa3f810">

The message is then broadcasted from the server to all UDP connected clients :)






