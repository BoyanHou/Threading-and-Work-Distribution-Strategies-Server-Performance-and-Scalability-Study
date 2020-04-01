#include "Client.h"

#include <time.h>  // for time() to seed rand()

#include <exception>  // pre-created thread catch all exceptions

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
  try {
    socket.connect_to(this->server_ip, this->server_port);
    //std::cout << "Connection success" << std::endl;
    this->send_request(socket, request);
    std::string response = this->recv_response(socket);
    //next line is for test
    //std::cout << "Response is:" << response << std::endl;
    socket.close_socket();
  }
  catch (std::exception * e) {
    std::cout << e->what() << std::endl;
    socket.close_socket();
  }
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

void Client::run_multi_thread(unsigned int thread_num,
                              unsigned int bucket_num,
                              unsigned int delay_min,
                              unsigned int delay_max) {
  std::vector<std::thread> threads;
  // create a certain number of threads
  for (unsigned int i = 0; i < thread_num; i++) {
    threads.push_back(
        std::thread(&Client::thread_func_rand, this, bucket_num, delay_min, delay_max));
  }

  for (unsigned int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }
}

void Client::thread_func(const std::string & request) {
  this->run_single(request);
}

void Client::thread_func_rand(unsigned int bucket_num,
                              unsigned int delay_min,
                              unsigned int delay_max) {
  srand((int)time(0));

  // first apply range-offet
  // then, minus 1 to convert from num to index
  unsigned int bucket_index_max =
      (unsigned int)(bucket_num * CLIENT_BUCKET_NUM_OFF_RANGE) - 1;

  while (1) {  // each thread ever-running
    // random bucket index
    unsigned int bucket_index = rand() % (bucket_index_max + 1);
    std::string bucket_index_str = std::to_string(bucket_index);

    // random delay count
    unsigned int delay_count = rand() % (delay_max - delay_min + 1) + delay_min;
    std::string delay_count_str = std::to_string(delay_count);

    // request
    std::string request = delay_count_str + "," + bucket_index_str + "\n";

    // communicate with server
    this->run_single(request);
  }
}
