#ifndef LoaderListModelH
#define LoaderListModelH

#include "LoadFromHDD.h"
#include <wtypes.h>

class TLoaderListModel
{

  TLoadFromHDD lfHDD;
  int mCntPath;

  int mSize;
  char* pData;


public:
  TLoaderListModel();
  ~TLoaderListModel();

  bool Open(char* sPath);
  bool Load(char*** pPointerArr,int& mCntPathModel);

protected:
  bool FindCnt();
  void ReadData();
  void ClearBuffer();
  char* FindSeparator(char* pBegin,int &size);
};
#endif