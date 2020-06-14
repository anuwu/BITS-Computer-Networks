#Computer Networks Assignment

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

The profile script is not perfect. Its output is indeterminate, probably due to server/relay/client tasks finishing in an unknown order. However, each probability is probed 10 times, which will give an output once. The profiled data will be present in `prof.txt` and one can use `gnuplot` to plot it.


