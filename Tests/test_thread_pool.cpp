#include "../Thread_Pool/Thread_Pool.h"
#include <iostream>

using namespace std; 

void p(std::string str) {
  std::cout<< str << std::endl;
}

int main () {
  std::threadpool pool{50};
  pool.commit(p, "hello").get();
}



