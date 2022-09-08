#include "DX_main.h"
#include "LoggerDX.h"
#include "LoadFromHDD.h"

void Test();

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
  Test();

  InitLoggerDX("Editor");

  TDX dx;
  dx.Work(NULL);

  //dx.Work(NULL);

  return 0;
}


#pragma pack(push, 1)
#if 0
struct TFileWOT
{
  char z0;
  char z1;
  float data[8];
  char r1;
  char r2;
  char r3;
};
#else
struct TFileWOT
{
  float data[8];
};
#endif
#pragma pack(pop)


void Test()
{
  TLoadFromHDD lfHDD;
  //lfHDD.ReOpen("D:\\MyProjects\\multiPlayer\\wot\\R04_T-34\\normal\\lod0\\Chassis.primitives");
    lfHDD.ReOpen("D:\\MyProjects\\multiPlayer\\wot\\env004_Telega\\normal\\lod0\\env004_Telega1.primitives");
  int size = lfHDD.Size();
  char* pWOT = new char[size];
  lfHDD.Read(pWOT,size);
#if 0
  int cnt = sizeof(TFileWOT);
  int bingo[sizeof(TFileWOT)];
  for(int i = 0 ; i < cnt ; i++)
  {
    TFileWOT* pFileWOT = (TFileWOT*)(pWOT+i);
    bingo[i] = 0;
    for(int j = 0; j < size/cnt ; j++)
    {
      if(pFileWOT[j].z0=='ÿ')
        bingo[i]++;
    }
  }
#endif
  TFileWOT* pFileWOT0 = (TFileWOT*)(pWOT+0);
  TFileWOT* pFileWOT25 = (TFileWOT*)(pWOT+25);

  unsigned short* pShort0 = (unsigned short*)(pWOT+0);
  unsigned short* pShort1 = (unsigned short*)(pWOT+1);
  unsigned short* pShort2 = (unsigned short*)(pWOT+2);
  unsigned short* pShort3 = (unsigned short*)(pWOT+3);
}
