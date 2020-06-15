# Networking Assignment

There are two problems in this assignment. In both problems, packets can be dropped with a probability of [0, 1) and this number can be set at runtime.

## Problem 1

A client opens 2 parallel TCP connections to a server and sends data to it. The protocol implemented is stop & wait. ACKs are sent back by the server.

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

A plot, roughly of similar nature, should be obtained (Caution : It might take 4-5 hours to obtain the plot)

![alt text][prof1]

[prof1]: https://github.com/anwesh0304/Computer-Networks/blob/master/Problem%201/profile.png "Profile of problem 1"  

## Problem 2

One client sends even and odd numbered packets to two relays, which inturn sends the data to the server. UDP sockets are used, and the protocol implemented is a sliding-window protocol with a timer (for the entire window).

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./relay <DROP_PROBABILITY> 1`	(Ensure that the probabilty mentioned here is the same as the one just above)
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

The following plot will be obtained (in 3-4 hours)


![alt text][prof2]

[prof2]: https://github.com/anwesh0304/Computer-Networks/blob/master/Problem%202/prof.png "Profile of problem 2"  

## Note

	- Parameters of the problem are described in `packet.h` in the respective folder
	- If socket binding fails, execute `netstat -nptul` to kill any process that's occupying port 8888
