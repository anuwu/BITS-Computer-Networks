# Networking Assignment

There are two problems in this assignment. In the first problem, one client opens 2 parallel TCP connections to a server and sends data to it. In the second problem, one client sends even and odd packets to two relays, which inturn sends the data to the server. Packets can be dropped with a probability of [0, 1) and this number can be set at runtime.

## Problem 1

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

## Problem 2

	- Run `make`
	- `./server <DROP_PROBABILITY> 1`
	- `./relay <DROP_PROBABILITY> 1`	(Ensure that the probabilty mentioned here is the same as the one just above)
	- `./client 1`

Run the `./profile.sh` script to profile the data transfer time with respect to the dropping probability.

## Note

For each probability, the file transfer is repeated multiple number of times to obtain a statistical average in steps of 0.05 probability starting from 0.05 upto 0.99. To obtain a decent profile, run `./profile.sh`. Otherwise, to obtain a finer profile in steps of 0.01, run `./profile_fine.sh`


