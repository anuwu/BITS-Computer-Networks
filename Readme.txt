## Problem 1

### How to run the program -

1. Run make 
2. Open two terminals.
3. Run make on one terminal.
4. First run ./server.exe on one terminal
5. Then run ./client.exe on the other terminal

### Notes

Input is stored in "input.txt". The downloaded file on the server-side is saved as "output.txt".
Dropping probability, timeout and packet size is defined in packet.h

### Methodology

1. Two processes are forked that send packets through both the channels simultaneously
2. ACKs or timeouts are monitored in each of the channels
3. Any packet that is timed-out is sent back on its corresponding channel

The way the server works -
1. It uses the select () function in C that monitors the various socket fds for any activity
2. Packet drop probability is set at 0.2
3. The incoming data is directly written at the appropriate position using the offset and SEEK_SET.


## Problem 2

### How to run the program

1. Open three terminals
2. Run make in one of them
3. Run ./server in first terminal
4. Run ./relay in second terminal
5. Run ./client in third terminal
6. The desired logs are in the three terminals.

### Notes

Input is stored in "input.txt". The downloaded file on the server-side is saved as "output.txt".
Dropping probability, timeout, window size and packet size is defined in packet.h

### Methodology

1. Two processes are forked that send packets through both the channels simultaneously
2. ACKs or timeouts are monitored in each of the channels
3. Any packet that is timed-out is sent back on its corresponding channel

The way the server works -
1. It uses the select () function in C that monitors the various socket fds for any activity
2. Packet drop probability is set at 0.2
3. The incoming data is directly written at the appropriate position using the offset and SEEK_SET.

