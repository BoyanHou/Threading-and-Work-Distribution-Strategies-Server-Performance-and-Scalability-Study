#include "Server.h"

Server::Server(int thread_mode,
               std::size_t bucket_num,
               const std::string & ip,
               const std::string & port) {
  this->buckets = std::vector<int>(bucket_num);
  this->thread_mode = thread_mode;
  this->server_socket = Socket();
  this->server_socket.bind_to(ip, port);
}

void Server::process_request(int client_socket_index) {
  Socket client_socket = this->client_sockets[client_socket_index];
  std::string request_str = client_socket.recv_str(CLIENT_BUF_SIZE);

  Server::Request request(request_str);

  if (request.bucket_index >= this->buckets.size()) {
    std::cout << "Invalid Bucket Index" << std::endl;

    return;
  }
}
