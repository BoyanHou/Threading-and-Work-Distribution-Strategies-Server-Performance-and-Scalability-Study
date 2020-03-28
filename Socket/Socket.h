#ifndef SOCKET_BH
#define SOCKET_BH

#include <arpa/inet.h>  // inet_ntop
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "../Tools/String_Tools.h"

class Socket {
 protected:
  int sock_fd;
  virtual struct addrinfo * get_addrinfo(const std::string & ip,
                                         const std::string & port);

 public:
  Socket();
  Socket(int fd);  // use socket fd to create a socket object
  ~Socket();       // close the socket fd when destructed

  // get the current port number of the socket
  virtual std::string get_socket_port();

  // send a string
  virtual void send(const std::vector<char> & content);
  // send a vector of chars
  virtual void send(const std::string & content);

  // receive a string
  virtual std::string recv_str(int buffer_size);

  // receive a vector of chars
  virtual std::vector<char> recv_vec(int buffer_size);

  // enter listen state
  virtual void set_listen(int back_log);

  // accept a connection & return the respawned socket
  virtual Socket * accept_connection();

  // connect to remote ip & port
  virtual void connect_to(const std::string & ip, const std::string & port);

  // bind socket to specific ip & port
  virtual void bind_to(const std::string & ip, const std::string & port);

  // allow port reuse
  virtual void allow_port_reuse();

  // get socket fd
  virtual int get_fd();

  //close socket
  virtual void close_socket();

  virtual std::string get_peer_ip();
};

#endif
