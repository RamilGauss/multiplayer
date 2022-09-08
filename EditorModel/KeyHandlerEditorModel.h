#ifndef KeyHandlerEditorModelH
#define KeyHandlerEditorModelH


#include "IKeyHandler.h"

class TKeyHandlerEditorModel : public IKeyHandler
{

public:
  TKeyHandlerEditorModel();
  virtual ~TKeyHandlerEditorModel();

  virtual unsigned int Work(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);

};


#endif

