///////////////////////////////////////////////////////////
//  LogManager.h
//  Implementation of the Class TLogManager
//  Created on:      29-май-2010 19:37:37
//  Original author: KIT
///////////////////////////////////////////////////////////

#if !defined(EA_B762BF23_D62E_41d5_9EDB_DDC4145FEB5D__INCLUDED_)
#define EA_B762BF23_D62E_41d5_9EDB_DDC4145FEB5D__INCLUDED_

#include "AutoCreateVar.h"
#include "BL_Log.h"
#include "ILogConsole.h"
#include "LogStd.h"
#include "GCS.h"
#include <list>

class GBASELIB_EI TLogManager : protected GCS
{
  TLogStdErr mLogStdErr;
  TLogStdOut mLogStdOut;
public:
	TLogManager();
	virtual ~TLogManager();

	void Register(ILogConsole* console);
	void Unregister(ILogConsole* console);

  void Link( TBL_Log* writer );
  void Link( TBL_Log* writer, const char* consoleName );
  void Unlink( TBL_Log* writer );

private:
  typedef std::list<ILogConsole*> TListConsole;
  typedef std::list<TLogWriter*>  TListWriter;                 

  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )
      TListConsole  mConsoles;
      TListWriter   mWriters;
    # pragma warning(pop)
  #else
    TListConsole  mConsoles;
    TListWriter   mWriters;
  #endif

  ILogConsole* Find( const char* name );
  ILogConsole* CheckConsole(const char* consoleName);
  void setGroupComment();
  void parseDescConsole( TBL_Log* writer, const char* descConsole );

};

extern GBASELIB_EI TAutoCreateVarT<TLogManager> LogManager;

#endif // !defined(EA_B762BF23_D62E_41d5_9EDB_DDC4145FEB5D__INCLUDED_)
