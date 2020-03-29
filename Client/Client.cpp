#include"Client.h"

Client::Client(const std::string &ip,  
	       const std::string &port){
  this->client_socket = Socket();
  this->client_socket.connect_to(ip,port);
}

void Client::send_request(const std::string& delay_count,const std::string& bucket_to_add){
  std::string to_send = delay_count;
  to_send += bucket_to_add;
  this->client_socket.send(to_send);
}

std::string Client::recv_response(){
  std::string response = this->client_socket.recv_str(SERVER_BUF_SIZE);
  return response;
}

void Client::run_client(const std::string& delay_count,const std::string& bucket_to_add){
  this->send_request(delay_count,bucket_to_add);
  std::string response = this->recv_response();
  //next line is for test
  std::cout<<"Response is:"<<response<<std::endl;
}
