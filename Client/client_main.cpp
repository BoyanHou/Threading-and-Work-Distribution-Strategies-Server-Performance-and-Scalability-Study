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

void run_client_multiple(Client & client,
                         unsigned int thread_num,
                         unsigned int bucket_num,
                         unsigned int delay_min,
                         unsigned int delay_max) {
  client.run_multi_thread(thread_num,
                          bucket_num,
                          delay_min,
                          delay_max);
  return;
}

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4 && argc != 7) {
    std::string usage = "Usage:./client <server_ip> <server_port>\n";
    usage += "      ./client <server_ip> <server_port> <test_file_path>\n";
    usage += "      ./client <server_ip> <server_port> <thread_num> <bucket_num> <delay_min> <delay_max>\n";

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
  else if (argc == 7) {
    unsigned int thread_num = 0;
    unsigned int bucket_num = 0;
    unsigned int delay_min = 0;
    unsigned int delay_max = 0;
    try {
      thread_num = String_Tools::str_to_unsigned_int(argv[3]);
      bucket_num = String_Tools::str_to_unsigned_int(argv[4]);
      delay_min = String_Tools::str_to_unsigned_int(argv[5]);
      delay_max = String_Tools::str_to_unsigned_int(argv[6]); 
    }
    catch (parse_unsigned_int_exception * e) {
      std::cout << e->what() << std::endl;
      return EXIT_FAILURE;
    }
    run_client_multiple(client,
                        thread_num,
                        bucket_num,
                        delay_min,
                        delay_max);
  }
  else {
    run_client_single(client);
  }
  return 0;
}
