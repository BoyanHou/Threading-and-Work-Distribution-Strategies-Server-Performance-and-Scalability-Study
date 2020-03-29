#ifndef SERVER_BH
#define SERVER_BH

#include <queue>

#include "../Socket/Socket.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

class Server {
 protected:
  std::vector<int> buckets;           // the "buckets" of this project
  Socket server_socket;               // the server socket
  std::queue<Socket> client_sockets;  // the accepted-spawned socket to clients
  int thread_mode;                    // either PRE_CREATE_THREADS, or PER~
  // process request from client
  void process_request(Socket & client_socket);

  class Request {
   public:
    unsigned int delay_count;
    unsigned int bucket_index;
    Request(const std::string & request_str);
  };

 public:
  // constructor
  Server(int thread_mode,
         std::size_t bucket_num,     // number of buckets
         const std::string & ip,     // the ip for server socket to bind to
         const std::string & port);  // the port for server socket to bind to

  // let the server run
  void run();

  void run_pre();  // run in pre-create mode

  void run_per();  // run in per-create mode
};

#endif
