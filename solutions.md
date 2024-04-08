## c++
### server with blocking accept and threads
```
       |
     listenSocket
       |
     socket()
       |
     bind()  
       |
     listen()
 ______|
|      |
|    accept() -> clientSocket --> thread(clientSocket)
|______|
```
### server with non blocking select and threads
(implemented solution)
- can handle other tasks in main loop
```
      listenSocket
        |
      socket()
        |
      bind()  
        |
      listen()
 _______|
|       |
|     fd_set
|       |
|     FD_ZERO()
|       | 
|     FD_SET()
|       |
|     select()
|       |
|     FD_ISSET()
|      |     |
|      N     Y
|      |     |
|      |   accept() -> clientSocket --> thread(clientSocket)
|      |_____|
|         |
|       other_tasks()
|_________|
```
### server with blocking accept in separate thread
(ChatGPT solution)
```
  |
  |----> thread
  |        |
  |      listenSocket
  |        |
  |      socket()
  |        |
  |      bind()  
  |        |
  |      listen()
  |    ____|
  |   |    |
  |   |  accept() -> clientSocket --> thread(clientSocket)
  |   |____|
  |
wait for input
  |
stop()
  |
join client threads
  |
```
## python
### server with select and threads
```
      listenSocket
        |
      socket()
        |
      bind()  
        |
      listen()
 _______|
|       |
|     select()
|      |   |
|      N   Y
|______|   |--> thread(listenSocket), accept() -> clientSocket
|__________|
```
