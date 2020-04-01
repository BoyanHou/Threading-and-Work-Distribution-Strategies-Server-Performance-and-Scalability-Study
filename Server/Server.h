#ifndef SERVER_BH
#define SERVER_BH

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

#include "../Socket/Socket.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

class Server {
 protected:
  std::mutex bucket_mutex;
  std::mutex socket_mutex;
  std::vector<std::atomic<int> > buckets;  // the "buckets" of this project
  Socket server_socket;                    // the server socket
  std::queue<Socket> client_sockets;       // the accepted-spawned socket to clients
  int thread_mode;                         // either PRE_CREATE_THREADS, or PER~
  std::atomic<int> req_count;              // count of total completed requests
  // process request from client
  void process_request(Socket client_socket);

  // hw-required way of adding delay to threads
  void required_delay(unsigned int req_delay);

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

  void run_pre();         // run in pre-create mode
  void run_pre_thread();  // func for pre-created thread to run

  void run_per();  // run in per-create mode
};

#endif
