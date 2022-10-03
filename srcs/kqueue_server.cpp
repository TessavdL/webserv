#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <string.h>
#include <unistd.h>

#include "../listening_sockets/SocketListen.hpp"

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int main()
{
	SocketListen		socket(9002);
	int					client_len;
    struct sockaddr_in	client_addr;

    client_len = sizeof(client_addr);

	//	CREATE KERNEL QUEUE
    int	kq = kqueue();
	if (kq == -1) {
		return (error_and_exit("An error occured in kqueue().\n"));
	}

    // Create event 'filter', these are the events we want to monitor.
    // Here we want to monitor: socket_listen_fd, for the events: EVFILT_READ 
    // (when there is data to be read on the socket), and perform the following
    // actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue 
    // and enable it).
    struct kevent	change_event[4];
    EV_SET(change_event, socket.getFd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    // REGISTER EVENT TO KERNEL QUEUE
    if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
		return (error_and_exit("An error occured in kevent() while trying to register the kernel event to the queue.\n"));
    }

    // EVENT LOOP
	int				socket_connection_fd;
	int				new_events;
	struct kevent	event[4];
    for (;;)
    {
        // CHECK FOR NEW EVENTS
		// Do not register new events with the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
        // Only handle 1 new event per iteration in the loop; 5th argument is 1.
        new_events = kevent(kq, NULL, 0, event, 1, NULL);
        if (new_events == -1) {
			return (error_and_exit("An error occured in kevent() while checking for new events.\n"));
        }

		// LOOP OVER NEW_EVENTS
        for (int i = 0; new_events > i; i++)
        {
            int event_fd = event[i].ident;

            // When the client disconnects an EOF is sent. By closing the file
            // descriptor the event is automatically removed from the kqueue.
            if (event[i].flags & EV_EOF) {
                printf("--- a client has disconnected ---");
                close(event_fd);
            }

            // If the new event's file descriptor is the same as the listening
            // socket's file descriptor, we are sure that a new client wants 
            // to connect to our socket.
            else if (event_fd == socket.getFd())
            {
                printf("--- a client has connected ---\n");
				// Incoming socket connection on the listening socket.
                // Create a new socket for the actual connection to client.
                socket_connection_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
                if (socket_connection_fd == -1) {
					return (error_and_exit("An error occured in accept()\n"));
                }

                // Put this new socket connection also as a 'filter' event
                // to watch in kqueue, so we can now watch for events on this
                // new socket.
                EV_SET(change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
                    return (error_and_exit("An error occured in kevent() when registering connected socket to the queue\n"));
                }
				printf("--- event was added to queue ---\n");
            }

			// READY TO READ FROM CLIENT SOCKET
            else if (event[i].filter & EVFILT_READ)
            {
				printf("--- reading from client socket ---\n");
                char buf[1024];
                size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
                printf("read %zu bytes\n", bytes_read);
				printf("buf = \n%s\n", buf);
				// temp: when using curl because otherwise it will stay connected
				close(socket_connection_fd);
				printf("--- done reading so bounce bye ---\n\n");
			}
        }
    }
    return (0);
}
