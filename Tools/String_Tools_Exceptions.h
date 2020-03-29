#ifndef STRT_EXPT
#define STRT_EXPT

#include <exception>

class file_not_found_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Cannot find file at designated path!";
  }
};

class parse_unsigned_int_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "The given string does not match an unsigned int!";
  }
};

class str_not_find_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Cannot find the position of the requested string!";
  }
};

#endif
