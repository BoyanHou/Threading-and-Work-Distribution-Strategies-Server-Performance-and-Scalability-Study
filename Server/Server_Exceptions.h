#ifndef SVR_EXC
#define SVR_EXC
#include <exception>

class request_format_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Request format is incorrect!";
  }
};

#endif
