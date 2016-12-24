# Progress

* Basic API of socket (create(), bind(), listen(), accept(), etc.)
* Basic Wrapper of the file descriptor
* Basic API of select and epoll
* Group division of epoll fd

# Schedule

## Dec 21st, 2016

### Work done

* Fixed some logical mistakes in addEpollEvent(), deleteEpollEvent(), modifyEpollEvent()
* Added the epoll\_ctl() to listen the upcoming connections of the socket
* Finished the framework of the test program of epoll API: epollAPITester.c

### Plan

* Check all the TODO mark
* Clarify the definition of Linux macro, and consider the usage
* Finish the test program of epoll API

## Dec 24th, 2016

### Work done

* Add the group of epoll fd(For listen, boardcast or other use)
* Finish the test program of epoll API;(Still didn't test yet) 
* Marco stuffs(No necessary to replace it)
* Count the TODO mark; *--5 TODO*

### Plan

* Epoll fd group manage(Create, delete)
* Test the test program of epoll API
* Add the change log of whole project
