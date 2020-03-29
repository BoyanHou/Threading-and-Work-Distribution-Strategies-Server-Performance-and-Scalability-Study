#ifndef CLIENT_KL
#define CLIENT_KL

#include <thread>

#include "../Socket/Socket.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

class Client {
 protected:
  std::string server_ip;
  std::string server_port;

  void send_request(Socket & socket, const std::string & request);
  std::string recv_response(Socket & socket);

  // create a socket to communicate with server regarding one request
  void thread_func(const std::string & request);

 public:
  // constructor
  Client(const std::string & ip,     //Server's ip
         const std::string & port);  //Server's port

  void run_single(const std::string & request);

  // run client for multiple threads
  // create one thread for each request
  void run_multi_thread(const std::vector<std::string> & requests);
};

#endif
