#include "LoaderModelDX.h"
#include "LoggerDX.h"

using namespace nsStruct3D;

TLoaderModelDX::TLoaderModelDX()
{

}
//--------------------------------------------------------------------------------
TLoaderModelDX::~TLoaderModelDX()
{

}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::Load(LPCWSTR strFilenameData)
{
  if(LoadMainFile()==false) 
  {
    GlobalLoggerDX.WriteF_time("�� ������� ��������� MainFile ��� ������.\n");
    return false;
  }
  //LoadFileResource();
  return false;
}
//--------------------------------------------------------------------------------
bool TLoaderModelDX::LoadMainFile()
{
  return false;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
