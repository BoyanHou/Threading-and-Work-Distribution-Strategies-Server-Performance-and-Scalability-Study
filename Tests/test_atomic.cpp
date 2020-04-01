#include <atomic>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

class Atomic_Tester {
 public:
  //int x;
  std::atomic<int> x;
  std::vector<int> results;
  unsigned int thread_num;

  Atomic_Tester(unsigned int thread_num) {
    this->x = 0;
    this->thread_num = thread_num;
    for (unsigned int i = 0; i < thread_num; i++) {
      results.push_back(0);
    }
  }

  void run_threads() {
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < this->thread_num; i++) {
      threads.push_back(
          std::thread(&Atomic_Tester::run_thread, this, 1000, std::ref(results[i])));
    }
    for (unsigned int i = 0; i < threads.size(); i++) {
      threads[i].join();
    }
    std::set<int> my_set;
    for (unsigned int i = 0; i < results.size(); i++) {
      int val = this->results[i];
      if (my_set.find(val) != my_set.end()) {
        std::cout << "Duplicate found: " << val << std::endl;
        this->print_vec();
        break;
      }
      else {
        my_set.insert(val);
      }
    }
  }

  void print_vec() {
    for (unsigned int i = 0; i < results.size(); i++) {
      std::cout << this->results[i] << " ";
    }
    std::cout << std::endl;
  }

  void run_thread(int num, int & result) {
    //    std::cout << "hey" << std::endl;
    // for (int i = 0; i < num; i++) {
    //   this->x++;
    // }
    result = x += num;
  }
};

int main() {
  for (int i = 0; i < 10000; i++) {
    Atomic_Tester tester(200);

    tester.run_threads();
    //  int val = tester.x;
    if (tester.x != 200000) {
      std::cout << "Result is: " << tester.x << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
