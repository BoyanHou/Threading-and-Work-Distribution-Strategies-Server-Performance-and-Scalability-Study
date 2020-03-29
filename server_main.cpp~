#include "./Server/Server.h"
#include "./Tools/String_Tools.h"
#include "./Tools/String_Tools_Exceptions.h"
#include "global_settings.h"

int main(int argc, char * argv[]) {
  std::string usage = "usage: ./server <bucket_num>";
  if (argc != 2) {
    perror(usage.c_str());
    return EXIT_SUCCESS;
  }

  std::string bucket_num_str(argv[1]);
  int bucket_num = 0;
  try {
    bucket_num = String_Tools::str_to_unsigned_int(bucket_num_str);
  }
  catch (parse_unsigned_int_exception * e) {
    perror(e->what());
    return EXIT_FAILURE;
  }

  Server server(PER_CREATE_THREADS, bucket_num, "0.0.0.0", SERVER_PORT);
}
