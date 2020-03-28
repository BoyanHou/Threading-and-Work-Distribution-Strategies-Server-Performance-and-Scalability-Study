#ifndef STRT_EXPT
#define STRT_EXPT

#include <exception>

class str_not_find_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Cannot find the position of the requested string!";
  }
};

#endif
