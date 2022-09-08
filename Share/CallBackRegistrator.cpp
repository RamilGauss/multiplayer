#include "ClientTank.h"
#include <stddef.h>
#include "memory_operation.h"
#include <string.h>
#include "BL_Debug.h"


CallBackRegistrator::CallBackRegistrator()
{
	mArrCallBack = NULL;
	mCntCallBack = 0;
}
//--------------------------------------------------------------
CallBackRegistrator::~CallBackRegistrator()
{

}
//--------------------------------------------------------------
void CallBackRegistrator::Register(TCallBackFunc pFunc)
{
	if(!InArr(pFunc))
		AddElement(pFunc);
	else
		BL_FIX_BUG();
}
//--------------------------------------------------------------
void CallBackRegistrator::Unregister(TCallBackFunc pFunc)
{
	for(int i = 0 ; i < mCntCallBack ; i++)
	{
		if(mArrCallBack[i]==pFunc)
    {
      RemoveInArr(i);
      return;
    }
	}
  BL_FIX_BUG();
}
//--------------------------------------------------------------
bool CallBackRegistrator::InArr(TCallBackFunc pFunc)
{
	for(int i = 0; i< mCntCallBack ; i++)
	{
		if(pFunc==mArrCallBack[i])
			return true;
	}
	return false;
}
//--------------------------------------------------------------
void CallBackRegistrator::AddElement(TCallBackFunc pFunc)
{
	int size = mCntCallBack*sizeof(TCallBackFunc);

	mArrCallBack = (TCallBackFunc(*))mo_realloc(mArrCallBack, size, size+sizeof(TCallBackFunc));

	mArrCallBack[mCntCallBack] = pFunc;
	mCntCallBack++;
}
//--------------------------------------------------------------
void CallBackRegistrator::RemoveInArr(int index)
{
	if(mCntCallBack==1){mCntCallBack=0;return;}

	if(mCntCallBack-1==index) {mCntCallBack--;return;}

	memmove(mArrCallBack+index,mArrCallBack+index+1,sizeof(TCallBackFunc)*(mCntCallBack-index-1));
	mCntCallBack--;
}
//--------------------------------------------------------------
void CallBackRegistrator::Notify(void* data, int size)
{
	for(int i = 0 ; i < mCntCallBack ; i++)
	{
		TCallBackFunc pFunc = mArrCallBack[i];
		pFunc(data,size);
	}
}
//--------------------------------------------------------------