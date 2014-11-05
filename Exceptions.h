#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
class IndexOutOfRangeException: public std::exception
{
  virtual const char* what() const throw() {return "Index out of range";}
};

class InvalidOperationException: public std::exception
{
    virtual const char* what() const throw() {return "Invalid operation";}
};
class InvalidArgumentException: public std::exception
{
    virtual const char* what() const throw() {return "Invalid arguments";}
};

#endif // EXCEPTIONS_H
