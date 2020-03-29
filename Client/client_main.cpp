#include <assert.h>

#include <string>

#include "Client.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    std::cerr << "Usage:./client_main [server ip] [server port]";
    return -1;
  }
  //Read request from cmd line
  std::string request;
  std::getline(std::cin, request);

  //Split the request
  std::vector<std::string> to_send = String_Tools::split_str(request, ",");

  //Ensure the second string doesn't include '\n'
  assert(to_send[1].find("\n") == std::string::npos);

  std::string ip(argv[1]), port(argv[2]);
  //Constructor
  Client client(ip, port);

  //Run client,send the 2 arguments
  client.run_client(to_send[0], to_send[1]);

  return 0;
}
