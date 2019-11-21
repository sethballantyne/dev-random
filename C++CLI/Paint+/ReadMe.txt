Paint+ is a simple network-based paint program I wrote to play around with SDL and SDL_net. The application will behave as a server or client depending on the command line arguments passed to it, and anything painted in the client will appear in the server in real-time and vice versa.

To start an instance of the server:
Paint+ <port>, where port is the port it should listen on.
To start an instance of the client:
Paint+ <ip> <port>, where ip is the address it should connect to and <port> is the port the server is listening on.
Examples:
Paint+ 4448 starts the server listening on port 4448
Paint+ localhost 4448 connects to the server @ 127.0.0.1:4448.

Keys 1 - 9 change the size of the brush.
R changes the brush to red, B to blue and G to green. 

A pre-built binary is in the zip. 
