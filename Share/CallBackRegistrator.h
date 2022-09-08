#ifndef CallBackRegistratorH
#define CallBackRegistratorH

class CallBackRegistrator
{

public:
	CallBackRegistrator();
	~CallBackRegistrator();

	typedef void(*TCallBackFunc)(void* data, int size);

protected:
	TCallBackFunc* mArrCallBack;
	int mCntCallBack;
public:

	void Notify(void* data, int size);
	void Register(TCallBackFunc pFunc);
	void Unregister(TCallBackFunc pFunc);
protected:
	bool InArr(TCallBackFunc pFunc);
	void AddElement(TCallBackFunc pFunc);
	void RemoveInArr(int index);
	//--------------------------------------------------------------------

};


#endif
