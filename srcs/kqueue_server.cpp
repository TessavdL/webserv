#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#include "../includes/listening_sockets/socket_listen.hpp"
#include "../includes/http_request_parser/http_request_lexer.hpp"
#include "../includes/http_request_parser/http_request_parser.hpp"

#define BUFF_SIZE 4096
#include <ctime>
#include <iomanip>

//Date: Mon, 27 Jul 2009 12:28:53 GMT

// static const char wday_name[][4] = {
//     "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
// };

// static const char mon_name[][4] = {
//    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
//    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
// };

// Wed Feb 13 15:46:11 2013

char* custom_asctime(const struct tm *timeptr)
{
  static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s,%3d %.3s %d %.2d:%.2d:%.2d CET\n",
    wday_name[timeptr->tm_wday],
	timeptr->tm_mday,
    mon_name[timeptr->tm_mon],
	1900 + timeptr->tm_year,
    timeptr->tm_hour,
    timeptr->tm_min,
	timeptr->tm_sec);
  return result;
}

std::string get_time(void) {
 	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime(&rawtime);
	std::string str("Date");
	str.append(": ");
	str.append(std::string(custom_asctime(timeinfo)));
	// str.append(std::to_string(wday_name[timeinfo->tm_wday]));
	// std::append(", ");
	// str.append(std::to_)
	// str.append(" ");
	// str.append(std::to_string(mon_name[timeinfo->tm_mday]));
	// str.append(" ");
	// str.append(std::to_string(1900 + timeinfo->tm_year));
	// str.append(" ")
	// str.append(std::to_string())
  return (str);
}

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
                printf("--- a client has disconnected ---\n");
                close(event_fd);
				// do not close socket_connection_fd, is bad file descriptor
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
            else if (event[i].filter == EVFILT_READ)
            {
				HTTPRequestLexer	lexer;
				long				total_amount_of_bytes_read = 0;
				int					bytes_read = 1;
				char				buf[BUFF_SIZE];

				std::cout << "--- reading from client socket ---" << std::endl;
                std::cout << "--- amount of bytes in data = " << event[i].data << "---" << std::endl;
	
				while (bytes_read > 0) {
					bytes_read = recv(event_fd, buf, sizeof(buf), 0);
					if (bytes_read == -1) {
						break ;
					}
					if (bytes_read > 0 && bytes_read < BUFF_SIZE) {
						std::cout << "--- finished reading from socket ---" << std::endl;
					}
					lexer.process_request(std::string(buf));
					if (lexer.get_state() == HTTPRequestLexer::REQUEST_START || lexer.get_state() == HTTPRequestLexer::REQUEST_ERROR) {
						std::cout << "--- request is invalid ---" << std::endl;
						break ;
					}
					total_amount_of_bytes_read += bytes_read;
				}

				if (total_amount_of_bytes_read == event[i].data) {
					std::cout << "--- all data send by the socket was received ---" << std::endl;
				}
                else {
					std::cout << "--- an error occured, not all send by the socket was received ---" << std::endl;
				}
				
				EV_SET(change_event, socket_connection_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
					return (error_and_exit("An error occured in kevent() when registering write event to the queue\n"));
				}
				printf("--- done reading ---\n");
			}
			else if (event[i].filter == EVFILT_WRITE) {
				printf("--- writing to client socket ---\n");
				std::string	str = "HTTP1/1 200 OK";
				str.append(CLRF);
				std::string date = get_time();
				str.append(date);
				str.append("Server: 42Webserv");
				str.append(CLRF);
				str.append(CLRF);
				const char *buf = str.c_str();
				// printf("sending this to client: %s", buf);


				send(event_fd, buf, strlen(buf), 0);
				printf("--- done writing to client socket\n");
				close(event_fd);
				printf("--- bounce bye ---\n\n");
			}
        }
    }
    return (0);
}
