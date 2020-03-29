#include "Server.h"

#include <sys/time.h>  // for "gettimeofday()"

#include "../Tools/String_Tools_Exceptions.h"
#include "Server_Exceptions.h"

Server::Server(int thread_mode,
               std::size_t bucket_num,
               const std::string & ip,
               const std::string & port) {
  this->buckets = std::vector<int>(bucket_num);
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
  while(true){//always accept client connection
    Socket client_socket = *((this->server_socket).accept_connection());
    //create a new thread to process request
    std::thread per_thread(&Server::process_request, this, client_socket);
    //detach from the main thread
    per_thread.detach();
  }
}

void Server::run_pre() {
  // pre-create threads
  for (int i = 0; i < PRE_THREAD_NUM; i++) {
    std::thread pre_thread(&Server::run_pre_thread, this);
  }
}

void Server::run_pre_thread() {
  while (1) {  // pre-created threads always run
    Socket client_socket;
    bool got_socket = false;
    // check for socket
    if (this->client_sockets.size() != 0) {
      // exception-safe lock
      std::unique_lock<std::mutex> guard(this->socket_mutex);
      client_socket = this->client_sockets.front();  // acquire
      this->client_sockets.pop();                    // remove
      // unlock
      guard.unlock();
      got_socket = true;
    }
    // process request if got a socket
    if (got_socket) {
      this->process_request(client_socket);
    }
  }
}

void Server::process_request(Socket client_socket) {
  std::string request_str = client_socket.recv_str(CLIENT_BUF_SIZE);
  try {
    Server::Request request(request_str);

    // bucket index too large
    if (request.bucket_index >= this->buckets.size()) {
      std::string err_str = "Invalid Bucket Index";
      perror(err_str.c_str());

      // send back failure info
      client_socket.send_str(err_str);

      return;
    }
    // add delay time in a required way
    this->required_delay(request.delay_count);

    // exception-safe lock
    std::unique_lock<std::mutex> guard(bucket_mutex);
    // process bucket value
    int bucket_val = this->buckets[request.bucket_index];
    bucket_val += request.delay_count;
    this->buckets[request.bucket_index] = bucket_val;
    // unlock
    guard.unlock();

    // send back info
    std::string bucket_val_str = std::to_string(bucket_val) + "\n";
    client_socket.send_str(bucket_val_str);
  }
  catch (request_format_exception * e) {
    perror(e->what());

    // send back failure info
    client_socket.send_str(e->what());
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
