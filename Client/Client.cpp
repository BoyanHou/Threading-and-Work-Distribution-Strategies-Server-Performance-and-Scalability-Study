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
  //  std::cout << "A client thread is trying to connect to server" << std::endl;
  socket.connect_to(this->server_ip, this->server_port);
  std::cout << "Connection success" << std::endl;
  this->send_request(socket, request);
  std::string response = this->recv_response(socket);
  //next line is for test
  std::cout << "Response is:" << response << std::endl;
}

void Client::run_multi_thread(const std::vector<std::string> & requests) {

  std::vector<std::thread> threads;
  for (unsigned int i = 0; i < requests.size(); i++) {
    std::string request = requests[i];
    threads.push_back(std::thread(&Client::thread_func, this, request));
  }

  for (unsigned int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }
}

void Client::thread_func(const std::string & request) {
  this->run_single(request);

}
