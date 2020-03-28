#ifndef PROXY_EXEPT
#define PROXY_EXEPT

#include <exception>

class get_addrinfo_exception : public std::exception {
  virtual const char * what() const throw() { return "Cannot get addrinfo!"; }
};

class get_socket_port_exception : public std::exception {
  virtual const char * what() const throw() { return "Cannot get socket port number!"; }
};

class connection_break_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Someone has broke your socket's connection!";
  }
};

class connect_to_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Socket cannot connect to that ip and port!";
  }
};

class recv_str_exception : public std::exception {
  virtual const char * what() const throw() { return "Socket failed to recv string!"; }
};

class send_str_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Socket cannot send your string to that ip and port!";
  }
};

class socket_listen_exception : public std::exception {
  virtual const char * what() const throw() { return "Socket cannot start listening!"; }
};

class socket_accept_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Cannot accept upcoming connection!";
  }
};

class socket_bind_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Cannot bind your socket to that ip and port!";
  }
};

class allow_port_reuse_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Failed to set the socket's port number as reusable!";
  }
};

#endif
