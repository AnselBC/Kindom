//
// Created by 宋辰伟 on 2017/4/5.
//

#ifndef PROJECT_SOURCELOCATION_H
#define PROJECT_SOURCELOCATION_H
// The SourceLocation class wraps up a source code location, including
// file name, function name, and line number, and contains a method to
// format the result into a string buffer.

#define MakeSourceLocation() SourceLocation(__FILE__, __FUNCTION__, __LINE__)

class SourceLocation
{
public:
  const char *file;
  const char *func;
  int line;

  bool
  valid() const
  {
    return file && line;
  }

  SourceLocation(const SourceLocation &rhs) : file(rhs.file), func(rhs.func), line(rhs.line) {}
  SourceLocation(const char *_file, const char *_func, int _line) : file(_file), func(_func), line(_line) {}
  SourceLocation &
  operator=(const SourceLocation &rhs)
  {
    this->file = rhs.file;
    this->func = rhs.func;
    this->line = rhs.line;
    return *this;
  }

  char *str(char *buf, int buflen) const;
};

#endif // PROJECT_SOURCELOCATION_H
