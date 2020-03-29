#include <assert.h>

#include <string>

#include "../Tools/String_Tools_Exceptions.h"
#include "Client.h"

void run_client_single(Client & client) {
  //Read request from cmd line
  std::string request;
  std::getline(std::cin, request);

  // //Split the request
  // std::vector<std::string> to_send = String_Tools::split_str(request, ",");

  // //Ensure the second string doesn't include '\n'
  // assert(to_send[1].find("\n") == std::string::npos);

  //Run client,send the 2 arguments
  client.run_single(request);

  return;
}

void run_client_multiple(Client & client, const std::vector<std::string> & requests) {
  client.run_multi_thread(requests);
  return;
}

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    std::string usage = "Usage:./client <server_ip> <server_port>\n";
    usage += "      ./client <server_ip> <server_port> <test_file_path>";

    std::cerr << usage << std::endl;
    return -1;
  }

  // create client
  std::string server_ip(argv[1]);
  std::string server_port(argv[2]);
  Client client(server_ip, server_port);

  if (argc == 4) {
    std::vector<std::string> requests;
    try {
      requests = String_Tools::read_vec_from_file(argv[3]);
      // de-escape the "\\"s back to "\"s
      for (unsigned int i = 0; i < requests.size(); i++) {
        std::string::size_type len = requests[i].length();
        if (len >= 2 && requests[i].substr(len-2).compare("\\n") == 0) {
          requests[i] = requests[i].substr(0, len-2);
          requests[i] += "\n";
        }
      }
    }
    catch (file_not_found_exception * e) {
      std::cout << e->what() << std::endl;
      return EXIT_FAILURE;
    }
    // for (unsigned int i = 0; i < requests.size(); i++) {
    //   std::cout << requests[i] << std::endl;
    // }
    run_client_multiple(client, requests);
  }
  else {
    run_client_single(client);
  }
  return 0;
}
