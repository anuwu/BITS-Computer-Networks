#Computer Networks Assignment

There are two problems in this assignment. In both problems, packets can be dropped with a probability of [0, 1) and this number can be set at runtime.

## Problem 1

A client opens 2 parallel TCP connections to a server and sends data to it. The protocol implemented is stop & wait. ACKs are sent back by the server.

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

## Problem 2

One client sends even and odd numbered packets to two relays, which inturn sends the data to the server. UDP sockets are used, and the protocol implemented is a sliding-window protocol with a timer (for the entire window).

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./relay <DROP_PROBABILITY> 1`	(Ensure that the probabilty mentioned here is the same as the one just above)
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

## Note

	- Parameters of the problem are described in `packet.h` in the respective folder
	- For each probability, the file transfer is repeated multiple number of times to obtain a statistical average in steps of 0.05 probability starting from 0.05 upto 0.99. To obtain a decent profile, run `./profile.sh`. Otherwise, to obtain a finer profile in steps of 0.01, run `./profile_fine.sh`
