Steps to run the solution to problem 1 -
1. Open two terminals.
2. Run make on one terminal.
3. First run ./server.exe on one terminal
4. Then run ./client.exe on the other terminal

Input is stored in "input.txt". The downloaded file on the server-side is saved as "output.txt".

The way client works -
1. Two processes are forked that send packets through both the channels simultaneously
2. ACKs or timeouts are monitored in each of the channels
3. Any packet that is timed-out is sent back on its corresponding channel

The way the server works -
1. It uses the select () function in C that monitors the various socket fds for any activity
2. Packet drop probability is set at 0.2
3. The incoming data is directly written at the appropriate position using the offset and SEEK_SET.
