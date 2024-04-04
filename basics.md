# c++
## sockets
### struct timeval
- VSC <WinSock2.h>
- GCC <sys/time.h>

```cpp
struct timeval 
{
   long tv_sec;    /* seconds */
   long tv_usec;   /* microseconds */
};
```
#### fd_set / select
- VSC <WinSock2.h>
- GCC <sys/select.h>

```cpp
//  reset fd_set
FD_ZERO(fd_set *fdset); 
//  assign socket (file descriptor) to fd_set
FD_SET(int fd, fd_set *fdset);
//  check if socket has event
FD_ISSET(int fd, fd_set *fdset);
//  clear  socket (file descriptor) in fd_set
FD_CLR(int fd, fd_set *fdset);

//  return: number of ready sockets
//  0: timeout reached
//  <0: error
int select(
  int     nfds,      // historical, unused
  fd_set* readfds,   // [in, out] 
  fd_set* writefds,  // [in, out] 
  fd_set* exceptfds, // [in, out] 
  const timeval * timeout // [in] 
);
```
### server with threads
```
      listenSocket
        |
      socket()
        |
      bind()  
        |
      listen()
________|
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
|      |    |
|      N    Y
|______|    |
|          accept() -> clientSocket, thread(clientSocket)
|___________|
```

## threads
### Start a thread with object methods
```cpp
#include <thread>
std::thread t(&Class::method, this, arguments..);
t.detach();
```


# python
### ipv6 example
```python
>>> import socket
>>> socket.getaddrinfo('www.python.org', 80, 0, 0, socket.SOL_TCP)
[(2, 1, 6, '', ('82.94.164.162', 80)),
 (10, 1, 6, '', ('2001:888:2000:d::a2', 80, 0, 0))]

>>> ourSocket = socket.socket(socket.AF_INET6, socket.SOCK_STREAM, 0)
>>> ourSocket.connect(('2001:888:2000:d::a2', 80, 0, 0))
```
### client example
```python
import socket

HOST = '127.0.0.1'
PORT = 8001

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hello, world')
    data = s.recv(1024)
    print(f'received {data}')
```
### server with threads
```
      listenSocket
        |
      socket()
        |
      bind()  
        |
      listen()
________|
|       |
|     select()
|      |   |
|      N   Y
|______|   |
|          |-> thread(listenSocket), accept() -> clientSocket
|__________|
```
