#include "Client.h"

Client::Client(const std::string & ip, const std::string & port) {
  this->client_socket.connect_to(ip, port);
}


void Client::send_request(const std::string & request) {
  this->client_socket.send_str(request);
}

std::string Client::recv_response() {
  std::string response = this->client_socket.recv_str(SERVER_BUF_SIZE);
  return response;
}

void Client::run_client(const std::string & request) {
  this->send_request(request);
  std::string response = this->recv_response();
  //next line is for test
  std::cout << "Response is:" << response << std::endl;
}