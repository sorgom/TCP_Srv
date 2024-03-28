
#### struct timeval
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
//  assign socket (file descriptor) to fd_set
FD_SET(int fd, fd_set *fdset);
//  clear  socket (file descriptor) to fd_set
FD_CLR(int fd, fd_set *fdset);
FD_ISSET(int fd, fd_set *fdset);
FD_ZERO(fd_set *fdset); 
```

```cpp
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


