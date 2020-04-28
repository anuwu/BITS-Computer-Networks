The packet drop probability is defined as #define DROP in packet.h
The window size is defined as #define WINDOW_SIZE in packet.h

Server port = 8888
Even relay port = 9000
Odd relay port = 9001

Steps to run the solution to Problem 2
1. Open three terminals
2. Run make in one of them
3. Run ./server in first terminal
4. Run ./relay in second terminal
5. Run ./client in third terminal

The timer running in client is for the entire window cycle and not for the individual packets sent.
