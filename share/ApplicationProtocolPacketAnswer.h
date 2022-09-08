#ifndef ApplicationProtocolPacketAnswerH
#define ApplicationProtocolPacketAnswerH

#include "ApplicationProtocolPacket.h"
#include "ApplicationProtocolDef.h"
#include "BL_Debug.h"
#include "memory_operation.h"



// APPL_TYPE_A_ECHO
class TA_Echo : public TBasePacket
{
	// ushort type
public:
	TA_Echo()
	{
		mType=APPL_TYPE_A_ECHO;
		mSize   = sizeof(mType);
		mData = (char*)malloc(mSize);
		setType();
	};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_TRY_CONNECT_TO_SERVER
class TA_Try_Connect_To_Server : public TBasePacket
{
	// ushort type, uchar code
	unsigned char mCode;
public:
	enum{eConnectGarage=nsServerStruct::TClient::eGarage, // есть соединение! в гараже
		eConnectWait=nsServerStruct::TClient::eWait,     // ждет
		eConnectFight=nsServerStruct::TClient::eFight,    // в бою
		eWasConnect,      // повторная попытка
		eBlackList,       // клиент в черном списке
		eOldVersion,      // старая версия клиента
    eOverloadServer,  // сервер перегружен
	};

public:
	TA_Try_Connect_To_Server(){mType=APPL_TYPE_A_TRY_CONNECT_TO_SERVER;};

	unsigned char getCode(){return *(getPointerCode());}; 
	unsigned char* getPointerCode(){return (unsigned char*)mData+sizeof(mType);}; 

	void setCode(unsigned char val)
	{
		mCode = val;
		if(mData==NULL)    
		{
			mSize = sizeof(mType)+sizeof(mCode);
			mData = (char*)malloc(mSize);
			setType();
		}
		unsigned char* pcode = getPointerCode();
		*(getPointerCode()) = mCode;
	};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_IN_FIGHT
class TA_In_Fight : public TBasePacket
{
	// ushort type, 
  // uchar code, 
  // ushort codeMap - код карты только если code==eFight
  // uchar - кол-во танков в команде
  // массив длинной CountTankInCommand*sizeof(DefTank)

  enum{
    // длина ника равна нулю
    eSizeDefTank = sizeof(unsigned char)/*lenNick*/+
                   sizeof(unsigned char)/*Gun*/+
                   sizeof(unsigned char)/*Tower*/+
                   sizeof(unsigned short)/*setID_Tank*/,
  };
public:
  enum{eFight=0,// в бой
    eWait,   // ждите
    eFailBlockTank, // танк нельзя использовать т.к. он еще находится в бою
    eFail,   // отказ выйти в бой, сбой в протоколе (отладка)
  };
  //-------------------------------------------------------------------------------------------
  TA_In_Fight()
  {
    mType=APPL_TYPE_A_IN_FIGHT;
    mSize = sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned short)+sizeof(unsigned char);
    mData = (char*)malloc(mSize);
    setType();
  }
  //-------------------------------------------------------------------------------------------
	unsigned char getCode(){return *getPointerCode();}; 
  void setCode(unsigned char val){*getPointerCode() = val;}
	unsigned char* getPointerCode(){return (unsigned char*)mData+sizeof(mType);}; 
  //-------------------------------------------------------------------------------------------
  unsigned short getCodeMap(){return *getPointerCodeMap();}
  void setCodeMap(unsigned short val){*getPointerCodeMap()=val;}
  unsigned short* getPointerCodeMap(){return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char));}
  //-------------------------------------------------------------------------------------------
  unsigned char getCountTank(){return *getPointerCountTank();}
  unsigned char* getPointerCountTank(){return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned short));}
  void setCountTank(unsigned char cnt)
  {
    *getPointerCountTank() = cnt;

    int newSize = sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned short)+sizeof(unsigned char)+
      cnt*eSizeDefTank;
    mData = (char*)mo_realloc(mData,mSize,newSize);
    mSize = newSize;
  }
  //-------------------------------------------------------------------------------------------
  bool getPointerStrNick(int val, char* buffer, int &len)
  {
    int cnt = getCountTank();
    char* pPointerFirstLenNick = getPointerBaseDefTank();
    char* pStrLenNickVal = pPointerFirstLenNick;
    for(int i = 0 ; i < cnt ; i++)
    {
      if(i==val)
      {
        len = *pStrLenNickVal;
        strcpy(buffer,pStrLenNickVal+sizeof(unsigned char));
        return true;
      }
      pStrLenNickVal += *pStrLenNickVal;// длина ника
      pStrLenNickVal += eSizeDefTank;
    }
    return false;
  }
  //-------------------------------------------------------------------------------------------
  void setPointerStrNick(int val, char* buffer )
  {
    int len = strlen(buffer)+1;
    int cnt = getCountTank();
    char* pPointerFirstLenNick = getPointerBaseDefTank();
    char* pStrLenNickVal = pPointerFirstLenNick;
    int size_bound = sizeof(mType)+sizeof(unsigned char)/*code*/+
       sizeof(unsigned short)/*code_map*/+
       sizeof(unsigned char)/*count*/+
       sizeof(unsigned char)/*длина переменной lenNick*/;
    for(int i = 0 ; i < cnt ; i++)
    {
      if(i==val)
      {
        goto l_mo_realloc;
      }
      size_bound += *pStrLenNickVal;
      size_bound += eSizeDefTank;
      pStrLenNickVal += *pStrLenNickVal;// длина ника
      pStrLenNickVal += eSizeDefTank;
    }   
    return;
l_mo_realloc:
    mData = mo_realloc_bound(mData,mSize,size_bound,len);
    mSize += len;

    pStrLenNickVal = mData+size_bound;
    *(pStrLenNickVal-sizeof(unsigned char)) = len;
    strcpy(pStrLenNickVal,buffer);
  }
  //-------------------------------------------------------------------------------------------
  unsigned char getGunType(int val){return *getPointerGunType(val);}
  void setGunType(int val,unsigned char gun){*getPointerGunType(val) = gun;}
  unsigned char* getPointerGunType(int val)
  {
    int cnt = getCountTank();
    char* pBaseDefTank = getPointerBaseDefTank();
    char* pDefTankVal = pBaseDefTank;
    for(int i = 0 ; i < cnt ; i++)
    {
      if(i==val)
      {
        return (unsigned char*)pDefTankVal         +
              sizeof(unsigned char)+// переменная "длина ника"
              (*pDefTankVal);// буффер под ник
      }
      pDefTankVal += *pDefTankVal;// длина ника
      pDefTankVal += eSizeDefTank;
    }   
    return NULL;// вместо BL_ASSERT
  }
  //-------------------------------------------------------------------------------------------
  unsigned char getTowerType(int val){return *getPointerTowerType(val);}
  void setTowerType(int val,unsigned char tower){*getPointerTowerType(val) = tower;}
  unsigned char* getPointerTowerType(int val)
  {
    int cnt = getCountTank();
    char* pBaseDefTank = getPointerBaseDefTank();
    char* pDefTankVal = pBaseDefTank;
    for(int i = 0 ; i < cnt ; i++)
    {
      if(i==val)
      {
        return (unsigned char*)pDefTankVal         +
          sizeof(unsigned char)+// переменная "длина ника"
          (*pDefTankVal)+// буффер под ник
          sizeof(unsigned char);// переменная тип пушки gun
      }
      pDefTankVal += *pDefTankVal;// длина ника
      pDefTankVal += eSizeDefTank;
    }   
    return NULL;// вместо BL_ASSERT
  }
  //-------------------------------------------------------------------------------------------
  unsigned short getID_Tank(int val){return *getPointerID_Tank(val);}
  void setID_Tank(int val,unsigned short id){*getPointerID_Tank(val) = id;}
  unsigned short* getPointerID_Tank(int val)
  {
    int cnt = getCountTank();
    char* pBaseDefTank = getPointerBaseDefTank();
    char* pDefTankVal = pBaseDefTank;
    for(int i = 0 ; i < cnt ; i++)
    {
      if(i==val)
      {
        unsigned short* pointer = (unsigned short*)((unsigned char*)pDefTankVal         +
          sizeof(unsigned char)+// переменная "длина ника"
          (*pDefTankVal)+// буффер под ник
          sizeof(unsigned char)+// переменная тип пушки gun
          sizeof(unsigned char));// переменная тип башни tower
        return pointer;
      }
      pDefTankVal += *pDefTankVal;// длина ника
      pDefTankVal += eSizeDefTank;
    }   
    return NULL;// вместо BL_ASSERT
  }
  //-------------------------------------------------------------------------------------------
protected:
  char* getPointerBaseDefTank(void)
  {
    return mData+sizeof(mType)+sizeof(unsigned char)/*code*/+
      sizeof(unsigned short)/*codeMap*/+sizeof(unsigned char)/*count*/;
  }
  //-------------------------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_FIRE_RELOAD
class TA_Fire_Reload : public TBasePacket
{
	// ushort type, float time_sec
	float mTime_sec;
public:
	TA_Fire_Reload(){mType=APPL_TYPE_A_FIRE_RELOAD;};

	float getTime(){return mTime_sec;}; 
	float* getPointerTime(){return (float*)(mData+sizeof(mType));}; 

	void setTime(float val)
	{
		mTime_sec = val;
		if(mData==NULL)    
		{
			mSize = sizeof(mTime_sec)+sizeof(mType);
			mData = (char*)malloc(mSize);
			setType();
		}
		float* pcode = getPointerTime();
		*pcode = mTime_sec;
	};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_GET_LIST_TANK
class TA_Get_List_Tank : public TBasePacket
{
  // ushort type, uchar cnt, {{ushort ID, uchar flgBlock}... }
public:
  TA_Get_List_Tank(){mType=APPL_TYPE_A_GET_LIST_TANK;};

  void setCnt(unsigned char cnt)
  {
    if(mSize==0)
    {
      mSize = sizeof(mType)+sizeof(cnt)+cnt*(sizeof(unsigned short)+sizeof(unsigned char));
      mData = (char*)malloc(mSize);
    }
    else
    {
      char* pTimeBuffer = (char*)malloc(mSize);
      memcpy(pTimeBuffer,mData,mSize);
      int oldSize = mSize;
      mSize = sizeof(mType)+sizeof(cnt)+cnt*(sizeof(unsigned short)+sizeof(unsigned char));
      free(mData);
      mData = (char*)malloc(mSize);
      memcpy(mData,pTimeBuffer,mSize>oldSize?oldSize:mSize);
      free(pTimeBuffer);
    }
    *getPointerCnt() = cnt;
    setType();
  };
  //-----------------------------------------------------------------------------
  unsigned char* getPointerCnt(){return (unsigned char*)(mData+sizeof(mType));};
  unsigned char getCnt(){return *getPointerCnt();};
  //-----------------------------------------------------------------------------
  void setTypeTank(unsigned char index, unsigned short val){*getPointerTypeTank(index) = val;}
  unsigned short* getPointerTypeTank(unsigned char index)
  {
    if(index>=getCnt()) 
    {
      BL_FIX_BUG();
      return NULL;
    }
    return (unsigned short*)(mData+sizeof(mType)+
      sizeof(unsigned char)+index*(sizeof(unsigned short)+sizeof(unsigned char)));
  };
  unsigned short getTypeTank(unsigned char index){return *getPointerTypeTank(index);};
  //-----------------------------------------------------------------------------
	void setFlgBlockTank(unsigned char index, unsigned char val){*getPointerFlgBlockTank(index) = val;}
	unsigned char* getPointerFlgBlockTank(unsigned char index)
	{
		if(index>=getCnt()) 
		{
			BL_FIX_BUG();
			return NULL;
		}
		return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char)+
      index*(sizeof(unsigned short)+sizeof(unsigned char))+sizeof(unsigned short));
	};
	unsigned char getFlgBlockTank(unsigned char index){return *getPointerFlgBlockTank(index);};
	//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// APPL_TYPE_A_END_FIGHT
class TA_End_Fight : public TBasePacket
{
  // ushort type, ХЗ пока неизвестно
  // uchar code - окончание боя, 
  // доп. инфо (пока неизвестно, reserved)

public:
  enum{eWin=0,
       eLoss=1,
       eNoOnes=2};

  TA_End_Fight()
  {
    mType=APPL_TYPE_A_END_FIGHT;
    mSize = sizeof(mType)+sizeof(unsigned char);
    mData = (char*)malloc(mSize);
    setType();
  }

  unsigned char getCode(){return *getPointerCode();};
  unsigned char* getPointerCode(){return (unsigned char*)(mData+sizeof(mType));};
  void setCode(unsigned char val){*getPointerCode()=val;};

};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_EXIT_WAIT
class TA_Exit_Wait : public TBasePacket
{
  // ushort type
public:
  TA_Exit_Wait()
  {
    mType = APPL_TYPE_A_EXIT_WAIT;
    mSize = sizeof(mType);
    mData = (char*)malloc(mSize);
    setType();
  }
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif