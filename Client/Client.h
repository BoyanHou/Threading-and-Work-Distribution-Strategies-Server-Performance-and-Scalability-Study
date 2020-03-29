#ifndef CLIENT_KL
#define CLIENT_KL

#include "../Socket/Socket.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

class Client {
 protected:
  Socket client_socket;  //the client socket
 public:
  // constructor
  Client(const std::string & ip,     //Server's ip
         const std::string & port);  //Server's port
  void run_client(const std::string & request);
  void send_request(const std::string & request);
  std::string recv_response();
};

#endif
