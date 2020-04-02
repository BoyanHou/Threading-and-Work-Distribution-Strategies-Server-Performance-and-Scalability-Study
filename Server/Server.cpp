#include "Server.h"

#include <sys/time.h>  // for "gettimeofday()"

#include "../Tools/String_Tools_Exceptions.h"
#include "Server_Exceptions.h"

Server::Server(int thread_mode,
               std::size_t bucket_num,
               const std::string & ip,
               const std::string & port) {
  this->buckets = std::vector<std::atomic<int> >(bucket_num);
  this->thread_mode = thread_mode;

  this->server_socket.bind_to(ip, port);
  this->server_socket.allow_port_reuse();

  this->server_socket.set_listen(BACK_LOG);
}

void Server::run() {
  if (this->thread_mode == PRE_CREATE_THREADS) {
    this->run_pre();
  }
  else if (this->thread_mode == PER_CREATE_THREADS) {
    this->run_per();
  }
  else {
    perror("Server mode not found\n");
  }
  return;
}

void Server::run_per() {
  //Timer is buggy, TODO
  struct timeval start, check;
  unsigned int runtime = SERVER_RUNTIME;  //run the server for <runtime> seconds
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do {  //always accept client connection
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) -
                      (start.tv_sec + (start.tv_usec / 1000000.0));
    Socket client_socket = *((this->server_socket).accept_connection());
    //create a new thread to process request
    std::thread per_thread(&Server::process_request, this, client_socket);
    //detach from the main thread
    per_thread.detach();
  } while (elapsed_seconds < runtime);
  std::cout << "In " << elapsed_seconds
            << " seconds, process requests: " << this->req_count << std::endl;
}

void Server::run_pre() {
  // // pre-create threads
  // for (int i = 0; i < PRE_THREAD_NUM; i++) {
  //   //std::cout << "creating queue No.: " << i << std::endl;
  //   // create a queue for each thread
  //   this->socket_queues.push_back(std::queue<Socket>());
  // }

  for (int i = 0; i < PRE_THREAD_NUM; i++) {
    std::thread pre_thread(&Server::run_pre_thread, this);
    pre_thread.detach();
  }

  struct timeval start, check;
  unsigned int runtime = SERVER_RUNTIME;  //run the server for <runtime> seconds
  double elapsed_seconds;
  gettimeofday(&start, NULL);

  // // use round-robin to assign socket to threads
  // int robin_index = 0;

  do {  // always receiving new connection

    //    robin_index = robin_index % PRE_THREAD_NUM;

    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) -
                      (start.tv_sec + (start.tv_usec / 1000000.0));

    //Socket client_socket = *((this->server_socket).accept_connection());
    // std::cout << "accepted once" << std::endl;

    // lock the socket queue
    // std::unique_lock<std::mutex> guard(this->socket_mutex);
    // this->client_sockets.push(client_socket);

    //this->socket_queues[robin_index].push(client_socket);

    //robin_index++;

  } while (elapsed_seconds < runtime);
  std::cout << "In " << elapsed_seconds
            << " seconds, process requests: " << this->req_count << std::endl;
}

void Server::run_pre_thread() {
  //  struct timeval start, check;
  // unsigned int runtime = SERVER_RUNTIME; //run the server for <runtime> seconds
  // double elapsed_seconds;
  //gettimeofday(&start,NULL);

  //  Socket client_socket;

  //do{
  while (1) {  // pre-created threads always run
    // gettimeofday(&check,NULL);
    // elapsed_seconds = (check.tv_sec + (check.tv_usec/1000000.0)) -
    //  (start.tv_sec + (start.tv_usec/1000000.0));

    // bool got_socket = false;

    // exception-safe lock
    std::unique_lock<std::mutex> guard(this->socket_mutex);
    Socket client_socket = *((this->server_socket).accept_connection());

    // // check for socket
    // if (socket_queue.size() != 0) {
    //   //std::cout << "Not 0! " << socket_queue.size() << std::endl;
    //   Socket client_socket = socket_queue.front();  // acquire
    //   socket_queue.pop();                           // remove
    //   // got_socket = true;
    //   this->process_request(client_socket);
    // }

    // unlock
    guard.unlock();
    this->process_request(client_socket);

    // process request if got a socket
    // if (got_socket) {
    //   this->process_request(client_socket);
    //   //client_socket.close_socket();
    // }
  }
}

void Server::process_request(Socket client_socket) {
  std::string request_str = client_socket.recv_str(CLIENT_BUF_SIZE);
  //std::cout << "Received " + request_str << std::endl;
  try {
    Server::Request request(request_str);

    // bucket index too large
    if (request.bucket_index >= this->buckets.size()) {
      std::string err_str = "Invalid Bucket Index: " + request.bucket_index;
      std::cout << (err_str.c_str()) << std::endl;

      // send back failure info
      client_socket.send_str(err_str);
      client_socket.close_socket();
      return;
    }
    // add delay time in a required way
    this->required_delay(request.delay_count);
    //std::cout << "processing " + request_str << std::endl;

    // // exception-safe lock
    // std::unique_lock<std::mutex> guard(bucket_mutex);
    // // process bucket value
    // int bucket_val = this->buckets[request.bucket_index];
    // bucket_val += request.delay_count;
    // this->buckets[request.bucket_index] = bucket_val;
    // // increment the req_count
    // this->req_count++;
    // // unlock
    // guard.unlock();

    // using c++ atomic int, lock free!
    int bucket_val = this->buckets[request.bucket_index] += request.delay_count;
    this->req_count++;

    // send back info
    std::string bucket_val_str = std::to_string(bucket_val) + "\n";
    client_socket.send_str(bucket_val_str);
    client_socket.close_socket();
  }
  catch (request_format_exception * e) {
    perror(e->what());

    // send back failure info
    client_socket.send_str(e->what());
    client_socket.close_socket();
    return;
  }
}

Server::Request::Request(const std::string & request_str) {
  // get rid of the "\n"
  std::vector<std::string> vec_no_slash_n = String_Tools::split_str(request_str, "\n");
  if (vec_no_slash_n.size() != 1) {
    throw new request_format_exception();
  }

  std::vector<std::string> vec = String_Tools::split_str(vec_no_slash_n[0], ",");
  if (vec.size() != 2) {
    throw new request_format_exception();
  }

  try {
    this->delay_count = String_Tools::str_to_unsigned_int(vec[0]);
    this->bucket_index = String_Tools::str_to_unsigned_int(vec[1]);
  }
  catch (parse_unsigned_int_exception * e) {
    perror(e->what());
    throw new request_format_exception();
  }
}

void Server::required_delay(unsigned int req_delay) {
  struct timeval start, check;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do {
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) -
                      (start.tv_sec + (start.tv_usec / 1000000.0));
  } while (elapsed_seconds < req_delay);
}
