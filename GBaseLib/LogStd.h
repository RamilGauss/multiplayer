#ifndef LogStdH
#define LogStdH

#include "ILogConsole.h"

class GBASELIB_EI TLogStdOut : public ILogConsole
{
public:
  TLogStdOut() {}
  virtual ~TLogStdOut() {}

public: // ILogConsole
  virtual const char* name() const  { return ":stdout"; }
  virtual void write(const char* str, int len = -1, bool eol = true );
};

class GBASELIB_EI TLogStdErr : public ILogConsole
{
public:
  TLogStdErr() {}
  virtual ~TLogStdErr() {}

public: // ILogConsole
  virtual const char* name() const  { return ":stderr"; }
  virtual void write(const char* str, int len = -1, bool eol = true );
};

#endif // !LogStdH
