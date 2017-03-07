# Progress

* Basic API of socket (create(), bind(), listen(), accept(), etc.)
* Basic Wrapper of the file descriptor
* Basic API of select and epoll
* Basic API of send & recv on client side.
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
* Count the TODO mark; *--2 TODO*

### Plan

* Epoll fd group manage(Create, delete)
* Test the test program of epoll API on puppy
* Add the change log of whole project

## Dec 25th, 2016

### Work done

* Finish the basic test of epoll API
* Change the file structure of the project
* Finish the header of client socket helper
* Count the TODO mark; *--3 TODO*

### Plan

* Add the change log of whole project
* Epoll fd group manage(Create, delete)
* Finish the client socket helper(Four core API: Asynchronous send and recv, chronous send and recv)

## Jan 6th, 2017

### Work done

* Finish the send & recv API of client side(except the asynchorouns send and recv base on AIO)
* Count the TODO mark; *--7 TODO*

### Plan

* Add the change log of whole project
* Epoll fd group manage(Create, delete)
* Finish the client socket helper(asynchronous send and recv base on AIO)
* Figure out reason of the redundant character of received message and fixed it

## Feb 3rd, 2017

### Work done

* Find the reason of redundant character(fgets bug) and fixed it

### Plan

* Epoll fd group manage(Create, delete)
* Finish the client socket helper(asynchronous send and recv base on AIO, or use threadpool)
* Add the change log of whole project
