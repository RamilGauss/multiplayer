///////////////////////////////////////////////////////////
//  LogFile.h
//  Implementation of the Class TLogFile
//  Created on:      29-май-2010 19:35:49
//  Original author: KIT
///////////////////////////////////////////////////////////

#if !defined(EA_C954E270_5042_4aad_B28A_46F9438FEBA6__INCLUDED_)
#define EA_C954E270_5042_4aad_B28A_46F9438FEBA6__INCLUDED_

#include "ILogConsole.h"
#include "BL_IOStreamFile.h"

class GBASELIB_EI TLogFile : public ILogConsole
{
public:
  TLogFile( const char* nameFile, bool enable = true );
	virtual ~TLogFile();

public: // ILogConsole
	virtual const char* name() const;
	virtual void write(const char* str, int len = -1, bool eol = true );

private:
	TIOStreamFile od;
  char* mName;
  bool mEnable;
  bool mCheckOpenFile;
  
  bool enable();
};
#endif // !defined(EA_C954E270_5042_4aad_B28A_46F9438FEBA6__INCLUDED_)
