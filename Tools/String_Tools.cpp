#include "String_Tools.h"

#include <stdlib.h>
#include <string.h>

#include <ctime>
#include <fstream>
#include <iostream>

#include "String_Tools_Exceptions.h"

std::vector<std::string> String_Tools::split_str(const std::string & str,
                                                 const std::string & delimiter) {
  std::string::size_type str_len = str.length();

  std::vector<std::string> parts;
  std::string::size_type start = 0;
  std::string::size_type end = start;
  do {
    end = str.find(delimiter, start);
    std::string part;
    if (end == std::string::npos) {
      if (start + 1 <= str_len) {
        part = str.substr(start, str_len - start);
        parts.push_back(part);
      }
      return parts;
    }
    else {
      part = str.substr(start, end - start);
      parts.push_back(part);
    }
    start = end + delimiter.length();
  } while (start + 1 <= str_len);

  return parts;
}

void String_Tools::replace_all(std::string & str,
                               const std::string & from_substr,
                               const std::string & to_substr) {
  std::string::size_type i;
  while((i = str.find(from_substr, i)) != std::string::npos) {
         str.replace(i, from_substr.length(), to_substr);
         i += to_substr.length();
  }
  
}

// read file content as vector of lines
std::vector<std::string> String_Tools::read_vec_from_file(const std::string & path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw new file_not_found_exception();
  }

  // read file line by line
  std::string line;
  std::vector<std::string> res;
  while (getline(file, line)) {
    res.push_back(line);
  }

  file.close();
  return res;
}

unsigned int String_Tools::str_to_unsigned_int(const std::string & str) {
  std::string::size_type len = str.length();
  if (len == 0) {
    throw new parse_unsigned_int_exception();
  }

  unsigned int res = 0;

  for (unsigned int i = 0; i < len; i++) {
    char ch = str[i];
    if (ch < '0' || ch > '9') {  // not digit
      throw new parse_unsigned_int_exception();
    }
    if (i == 0 && ch == '0' && len > 1) {  // leading 0
      throw new parse_unsigned_int_exception();
    }
    int num = (int)(ch - '0');
    res += res * 10 + num;
  }
  return res;
}

void String_Tools::load_into_vector(std::vector<char> & container,
                                    const std::string & str) {
  for (std::string::size_type i = 0; i < str.length(); i++) {
    container.push_back(str[i]);
  }
}

void String_Tools::load_chars_into_vector(std::vector<char> & container,
                                          char * chars,
                                          int size) {
  for (int i = 0; i < size; i++) {
    container.push_back(chars[i]);
  }
}

char * String_Tools::vector_to_chars(const std::vector<char> & container) {
  char * chars = (char *)malloc(container.size());

  memset(chars, 0, container.size());

  for (unsigned int i = 0; i < container.size(); i++) {
    chars[i] = container[i];
  }

  return chars;
}

void String_Tools::copy_vector(const std::vector<char> & target,
                               std::vector<char> & copy) {
  //std::cout << "COPIED VEC:\n";
  for (size_t i = 0; i < target.size(); i++) {
    copy.push_back(target[i]);
    //    std::cout << target[i];
  }
  // std::cout << "COPIED VEC COMPLETE\n";
}

std::string String_Tools::get_nowtime() {
  time_t nowtime = time(0);

  tm * gmt_nowtime = gmtime(&nowtime);
  char * dt = asctime(gmt_nowtime);
  std::string res(dt);
  return res;
}

struct tm String_Tools::rqs_to_tm(const std::string & str) {
  struct tm tm;
  memset(&tm, 0, sizeof(struct tm));
  const char * chars = str.c_str();
  strptime(chars, "%a, %d %b %Y %H:%M:%S %z", &tm);
  return tm;
}

time_t String_Tools::rqs_to_time_t(const std::string & str) {
  struct tm tm = String_Tools::rqs_to_tm(str);
  return mktime(&tm);
}

int String_Tools::find_int_after(const std::string & str, const std::string & target) {
  std::string::size_type pos = str.find(target) + target.length();
  std::string::size_type sz;
  int res = std::stoi(str.substr(pos), &sz);
  return res;
}
