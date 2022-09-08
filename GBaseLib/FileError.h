#ifndef FileErrorH
#define FileErrorH

#include "ErrorReg.h"
//DECLARE_CLASS( FileError )

///////////////////////////////////////////////////////////////////////////
/////////////////// Класс регистрации ошибок файла  ///////////////////////
///////////////////////////////////////////////////////////////////////////

class GBASELIB_EI TFileError
{
protected:
  char name[MAX_PATH];
  void *errSource;
public:
  TFileError( const char *_name = NULL );

  void SetFileName( const char *_name );
  const char* GetFileName() const { return name; }

  void SetErrSource( void* _errSource )
  { errSource = _errSource; }

  void SetError( guint8 group, guint32 code, const char* file, int line );
  void SetErrorStr( const char* cp, const char* text, const char* file, int line );
  void SetErrorSTD( guint32 code, const char* file, int line );
};

//-----------------------------------------------------------------------------
#define ODFfile_SetError( group, code )        \
  SetError( group, code, __FILE__, __LINE__ )

#define ODFfile_SetErrorStr( cp, text )        \
  SetErrorStr( cp, text, __FILE__, __LINE__ )

#define ODFfile_SetErrorFileSystem()           \
{ if( errno ) SetErrorSTD( errno, __FILE__, __LINE__ ); }
//-----------------------------------------------------------------------------

#endif // FileErrorH
