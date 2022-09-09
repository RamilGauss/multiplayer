#include <conio.h>
#include <stdio.h>
#include <map>
#include <string>
#include <windows.h>
#include "../Share/ContextMachine.h"
#include "../Share/StateMachine.h"

using namespace std;

int main(int argc, char** argv)
{
  string* s = new string;
  s->insert(0,"asdadasda");
  //TMarkUpContainer::TCommonDesc desc;
  //vector<TMarkUpContainer::TCommonDesc> vec;

  //desc.type = TMarkUpContainer::eConst;
  //desc.name = "1";
  //desc.c.size = 5;
  //vec.push_back(desc);
  //
  //desc.type = TMarkUpContainer::eConst;
  //desc.name = "2";
  //desc.c.size = 5;
  //vec.push_back(desc);

  //desc.type = TMarkUpContainer::eVar;
  //desc.name = "3";
  //desc.v.sizeCnt = 2;
  //desc.v.sizeVar = 1;
  //vec.push_back(desc);

  //desc.type = TMarkUpContainer::eVar;
  //desc.name = "4";
  //desc.v.sizeCnt = 2;
  //desc.v.sizeVar = 1;
  //vec.push_back(desc);

  //TMarkUpContainer container;

  //container.SetMarkUp(&vec);
  //void* ptr = container.Get("1");
  //ptr = container.Get("2");
  //// увеличиваем
  //ptr = container.Get("3");
  //*((short*)ptr) = 2;
  //ptr = container.Get("4");
  //*((short*)ptr) = 2;
  //container.Update();
  //ptr = container.Get("3",false,0);
  //*((char*)ptr) = 1;
  //ptr = container.Get("3",false,1);
  //*((char*)ptr) = 2;
  //ptr = container.Get("4",false,0);
  //*((char*)ptr) = 1;
  //ptr = container.Get("4",false,1);
  //*((char*)ptr) = 2;
  //// уменьшаем
  ////ptr = container.Get("3");
  ////*((short*)ptr) = 0;
  //container.Update();

  //TMarkUpContainer container_empty;
  //container_empty.SetMarkUp(&vec);
  //container_empty.Set(container.GetPtr(),container.GetSize());

  //DWORD start = GetTickCount();
  //start = GetTickCount()- start;

  //typedef TContextMachine<int,int> TStateMachineInt;

  //TStateMachineInt::TMapKeyValue mkv1, mkv2;

  //for(int i = 0 ; i < 50 ; i++)
  //  mkv2.insert(TStateMachineInt::TMapKeyValue::value_type(i,i));


  //TStateMachineInt sm;
  //sm.AddState(0,&mkv1);

  //for(int i = 0 ; i < 10 ; i++)
  //  sm.AddState(1,&mkv2);
  //
  //int answer = -1;
  //bool res = false;

  //res = sm.Action(0,answer);
  //res = sm.Action(1,answer);
  //res = sm.Action(1,answer);
  //res = sm.Action(1,answer);

  ////int* pInt = sm.Get<int>(1);

  //int cnt = 500000;

  //DWORD start = GetTickCount();
  //
  //for(int i = 0 ; i < cnt ; i++)
  //{
  //  //TStateMachineInt::TMapKeyValue::iterator fit = mkv2.find(10);
  //  //if(fit!=mkv2.end())
  //  //{
  //  //  int a = 0;
  //  //}
  //  res = sm.Action(1,answer);
  //}

  //start = GetTickCount()- start;

  typedef TStateMachine<int,int> TSM_int;
  TSM_int sm;
  int data = 10;
  TSM_int::TMapKeyStr mKS;
  mKS.insert(TSM_int::TMapKeyStr::value_type(0,"Garage"));
  mKS.insert(TSM_int::TMapKeyStr::value_type(1,"Wait"));
  mKS.insert(TSM_int::TMapKeyStr::value_type(2,"Fight"));
  sm.AddState("Main",&mKS, &data);
  mKS.clear();
  //-----------------------------------------------------
  mKS.insert(TSM_int::TMapKeyStr::value_type(0,"Main"));
  mKS.insert(TSM_int::TMapKeyStr::value_type(1,"Wait"));
  sm.AddState("Garage",&mKS, &data);
  mKS.clear();
  //-----------------------------------------------------
  mKS.insert(TSM_int::TMapKeyStr::value_type(0,"Garage"));
  mKS.insert(TSM_int::TMapKeyStr::value_type(1,"Fight"));
  sm.AddState("Wait",&mKS, &data);
  mKS.clear();
  //-----------------------------------------------------  
  mKS.insert(TSM_int::TMapKeyStr::value_type(0,"Garage"));
  sm.AddState("Fight",&mKS, &data);
  //-----------------------------------------------------
  sm.Init("Main");
  int* p = sm.Action(2);
  p = sm.Action(2);

  int cnt = 500001;
  DWORD start = GetTickCount();
  
  for(int i = 0 ; i < cnt ; i++)
    sm.Action(0);

  start = GetTickCount()- start;
  
  string name = sm.GetNameCurrent();
  p = sm.GetDataCurrent();

  printf("Result: %f mks, name=%s\n",float(start)/cnt*1000,name.data());
  _getch();
  return 0;
}
