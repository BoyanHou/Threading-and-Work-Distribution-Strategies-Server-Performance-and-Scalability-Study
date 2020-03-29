#include "Server.h"

#include "../Tools/String_Tools_Exceptions.h"
#include "Server_Exceptions.h"

Server::Server(int thread_mode,
               std::size_t bucket_num,
               const std::string & ip,
               const std::string & port) {
  this->buckets = std::vector<int>(bucket_num);
  this->thread_mode = thread_mode;

  this->server_socket.bind_to(ip, port);
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
  //TODO
  Socket client_socket = *((this->server_socket).accept_connection());
  this->process_request(client_socket);
}

void Server::run_pre() {
  //TODO
}

void Server::process_request(Socket & client_socket) {
  std::string request_str = client_socket.recv_str(CLIENT_BUF_SIZE);
  try {
    Server::Request request(request_str);

    // bucket index too large
    if (request.bucket_index >= this->buckets.size()) {
      std::string err_str = "Invalid Bucket Inde";
      perror(err_str.c_str());

      // send back failure info
      client_socket.send_str(err_str);

      return;
    }

    // process bucket value
    int bucket_val = this->buckets[request.bucket_index];
    bucket_val += 1;
    this->buckets[request.bucket_index] = bucket_val;

    // send back info
    std::string bucket_val_str = std::to_string(bucket_val);
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
