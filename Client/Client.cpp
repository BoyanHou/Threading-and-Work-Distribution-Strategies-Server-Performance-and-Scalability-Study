#include "Client.h"

Client::Client(const std::string & ip, const std::string & port) {
  this->server_ip = ip;
  this->server_port = port;
}

void Client::send_request(Socket & socket, const std::string & request) {
  socket.send_str(request);
}

std::string Client::recv_response(Socket & socket) {
  std::string response = socket.recv_str(SERVER_BUF_SIZE);
  return response;
}

void Client::run_single(const std::string & request) {
  Socket socket;
  socket.connect_to(this->server_ip, this->server_port);
  this->send_request(socket, request);
  std::string response = this->recv_response(socket);
  //next line is for test
  std::cout << "Response is:" << response << std::endl;
}

void Client::run_multi_thread(const std::vector<std::string> & requests) {
  for (unsigned int i = 0; i < requests.size(); i++) {
    std::string request = requests[i];
    std::thread th(&Client::thread_func, this, request);
    th.detach();
  }
}

void Client::thread_func(const std::string & request) {
  this->run_single(request);
}
