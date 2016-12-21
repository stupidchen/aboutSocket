# Progress
* Basic API of socket (create(), bind(), listen(), accept(), etc.)
* Basic Wrapper of the file descriptor
* Basic API of select and epoll

# Schedule

## Dec 21, 2016

### Work done
* Fixed some logical mistakes in addEpollEvent(), deleteEpollEvent(), modifyEpollEvent()
* Added the epoll\_ctl() to listen the upcoming connections of the socket
* Finished the framework of the test program of epoll API: epollAPITester.c

### Plan
* Check all the TODO mark;
* Clarify the definition of Linux macro, and consider the usage;
* Finish the test program of epoll API;
