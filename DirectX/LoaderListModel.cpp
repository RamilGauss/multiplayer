#include "LoaderListModel.h"
#include "BL_Debug.h"


TLoaderListModel::TLoaderListModel()
{
  mCurDirectory[0] = '\0';
  mSize = 0;
  pData = NULL;
}
//--------------------------------------------------------------------------------------
TLoaderListModel::~TLoaderListModel()
{
  ClearBuffer();
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::Open(char* sPath)
{
  GetCurrentDirectoryA(sizeof(mCurDirectory),mCurDirectory);
  int len = strlen(mCurDirectory);
  for(int i = len-1 ; i >= 0 ; i--)
  {
    if(mCurDirectory[i]=='\\')
    {
      mCurDirectory[i]='\0';
      break;
    }
  }

  bool res = lfHDD.ReOpen(sPath);
  if(!res) return false;

  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::Load(char*** pPointerArr,int& mCntPathModel)
{
  ReadData();

  mCntPath = 0;
  // ищем кол-во записей
  if(!FindCnt()) return false;
  
  char** pArr = new char*[mCntPath];
  *pPointerArr = pArr;

  int size = 0;
  char* p = FindSeparator(pData,size);// пропустить значение кол-ва
  p = FindSeparator(p,size);
  int i = 0;
  while(p)
  {
    if(size)
    {
      int lenStr = size+strlen(mCurDirectory)+strlen("\\model\\");
      pArr[i] = new char[lenStr+1];
      memcpy(pArr[i],mCurDirectory,strlen(mCurDirectory));
      memcpy(pArr[i]+strlen(mCurDirectory),"\\model\\",strlen("\\model\\"));
      memcpy(pArr[i]+strlen(mCurDirectory)+strlen("\\model\\"),p,size);
      char* str = pArr[i];
      str[lenStr] = '\0';
      i++;
    }
    p = FindSeparator(p,size);
  }

  if(i!=mCntPath) return false;

  mCntPathModel = mCntPath;
  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::FindCnt()
{
  int size = 0;
  char* p = FindSeparator(pData,size);
  if(p==NULL) return false;
  char buffer[300];
  memcpy(buffer,p,size);
  buffer[size] = '\0';
  mCntPath = atoi(buffer);
  if(mCntPath==0) return false;
  return true;
}
//--------------------------------------------------------------------------------------
void TLoaderListModel::ReadData()
{
  ClearBuffer();

  mSize = lfHDD.Size();
  pData = new char[mSize];
  lfHDD.Read(pData,mSize);
}
//--------------------------------------------------------------------------------------
void TLoaderListModel::ClearBuffer()
{
  mSize = 0;
  delete []pData;
  pData = NULL;
}
//--------------------------------------------------------------------------------------
char* TLoaderListModel::FindSeparator(char* pBegin,int &size)
{
  // *запись0*запись1*...
  char* pFound = NULL;
  int i = (int)(pBegin-pData);
  BL_ASSERT(i>=0);
  for(; i < mSize ; i++)
  {
    if(pData[i]=='*')
    {
      i++;
      pFound = &pData[i];
      for( int j = i ;j < mSize ; j++)
      {
        if(pData[j]=='*')
        {
          size = j - i;
          return pFound;
        }
      }
      size = mSize - i;
      return pFound;
    }
  }
  return pFound;
}
//--------------------------------------------------------------------------------------