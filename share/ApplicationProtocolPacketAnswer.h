/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


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
	enum{eConnectGarage=0, /*nsServerStruct::TClient::eGarage,*/ // есть соединение! в гараже
		eConnectWait/*=nsServerStruct::TClient::eWait*/,     // ждет
		eConnectFight/*=nsServerStruct::TClient::eFight*/,    // в бою
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
  // uchar  code (см. enum{eFight..), 
  // ushort codeMap - код карты только если code==eFight
  // uchar - кол-во танков в бою
  //--------------------------------------
  // ushort - "длина" информации о танке после поля "длина" получен GetSizeProperty()
  // массив инфы танка - получен GetProperty()
public:
  enum{eFight=0,// в бой
       eWait,   // ждите
       eFailBlockTank, // танк нельзя использовать т.к. он еще находится в бою
       eFail,   // отказ выйти в бой, сбой в протоколе (отладка)
  };
  enum{eLenHeader = sizeof(unsigned short)+
                    sizeof(unsigned char)+
                    sizeof(unsigned short)+
                    sizeof(unsigned char),
  };
  //-------------------------------------------------------------------------------------------
  TA_In_Fight();
  virtual ~TA_In_Fight(){};
  //-------------------------------------------------------------------------------------------
	unsigned char getCode();
  void setCode(unsigned char val);
	unsigned char* getPointerCode();
  //-------------------------------------------------------------------------------------------
  unsigned short getCodeMap();
  void setCodeMap(unsigned short val);
  unsigned short* getPointerCodeMap();
  //-------------------------------------------------------------------------------------------
  unsigned char getCountTank();
  //-------------------------------------------------------------------------------------------
  unsigned char* getPointerTankProperty(int i);
  unsigned short getSizeTankProperty(int i);

  void addTankProperty(unsigned char* pData, unsigned short size);
protected:
  unsigned char* FindTank(int i);

  void setCountTank(unsigned char cnt);
  unsigned char* getPointerCountTank();
};
//-----------------------------------------------------------------------------
//APPL_TYPE_G_A_FIRE_RELOAD
class TA_Fire_Reload : public TBasePacket
{
	// ushort type, float time_sec
	float mTime_sec;
public:
	TA_Fire_Reload(){mType=APPL_TYPE_G_A_FIRE_RELOAD;};

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
  // ushort type, 
  // uchar cnt, 
  // {{ushort ID, 
  //   uchar flgBlock}... }
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
//APPL_TYPE_A_CORRECT_PACKET_STATE_OBJECT
class TA_Correct_Packet_State_Object : public TBasePacket
{
  // ushort type
  // uchar cnt_object не более eMaxCountObject
  
  // ushort ID
  // uint maskState

public:
  enum{eMaxCountObject=100,
       eSizeDefObject = sizeof(unsigned short)+sizeof(unsigned int),
  };

  TA_Correct_Packet_State_Object()
  {
    mType = APPL_TYPE_G_A_CORRECT_PACKET_STATE_OBJECT;
    mSize = sizeof(mType);
    mData = (char*)malloc(mSize);
    setType();
  }
  //----------------------------------------------------------------------
  void setCountObject(unsigned char cnt)
  {
    int new_size = sizeof(mType)+sizeof(unsigned char)+cnt*eSizeDefObject;
    mData = (char*)mo_realloc(mData,mSize,new_size);
    mSize = new_size;
    
    unsigned char* pCnt = getPointerCountObject();
    *pCnt = cnt;
  }
  unsigned char  getCountObject(){return *getPointerCountObject();}
  unsigned char* getPointerCountObject(){return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char));}
  //----------------------------------------------------------------------
  unsigned short* getPointerID(int i)
  {return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefObject);}
  unsigned short getID(int i){return *getPointerID(i);}
  void setID(int i,unsigned short val){*getPointerID(i) = val;}
  //----------------------------------------------------------------------
  unsigned int* getPointerMaskState(int i)
  {return (unsigned int*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefObject+sizeof(unsigned short));}
  unsigned int getMaskState(int i){return *getPointerMaskState(i);}
  void setMaskState(int i,unsigned int val){*getPointerMaskState(i) = val;}
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//APPL_TYPE_A_CORRECT_PACKET_STATE_TANK
// в начале перед боем на обратном отсчете показать координаты и ориентацию танков
class TA_Correct_Packet_State_Tank : public TBasePacket
{
  // ushort type
  // uchar cnt_tank

  // uchar id - порядковый номер  танка
  // float x - координаты
  // float y
  // float z
  // float xv - ориентация
  // float yv
  // float zv

public:
  enum{
    eSizeDefTank = sizeof(unsigned char)
    +sizeof(float)+sizeof(float)+sizeof(float)
    +sizeof(float)+sizeof(float)+sizeof(float),
  };

  TA_Correct_Packet_State_Tank()
  {
    mType = APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK;
    mSize = sizeof(mType)+sizeof(unsigned char);
    mData = (char*)malloc(mSize);
    setType();
  }
  //----------------------------------------------------------------------
  void setCountTank(unsigned char cnt)
  {
    int new_size = sizeof(mType)+sizeof(unsigned char)+cnt*eSizeDefTank;
    mData = (char*)mo_realloc(mData,mSize,new_size);
    mSize = new_size;

    unsigned char* pCnt = getPointerCountTank();
    *pCnt = cnt;
  }
  unsigned char  getCountTank(){return *getPointerCountTank();}
  unsigned char* getPointerCountTank(){return (unsigned char*)(mData+sizeof(mType));}
  //----------------------------------------------------------------------
  unsigned char* getPointerID(int i)
  {return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank);}
  unsigned char getID(int i){return *getPointerID(i);}
  void setID(int i,unsigned char val){*getPointerID(i) = val;}
  //----------------------------------------------------------------------
  float* getPointerX(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char));}
  float getX(int i){return *getPointerX(i);}
  void setX(int i,float val){*getPointerX(i) = val;}
  //----------------------------------------------------------------------
  float* getPointerY(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char)+
                                       sizeof(float));}
  float getY(int i){return *getPointerY(i);}
  void setY(int i,float val){*getPointerY(i) = val;}
  //----------------------------------------------------------------------
  float* getPointerZ(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char)+
                                       sizeof(float)+sizeof(float));}
  float getZ(int i){return *getPointerZ(i);}
  void setZ(int i,float val){*getPointerZ(i) = val;}
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  //----------------------------------------------------------------------
  float* getPointerXV(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char)+
                                       sizeof(float)+sizeof(float)+sizeof(float));}
  float getXV(int i){return *getPointerXV(i);}
  void setXV(int i,float val){*getPointerXV(i) = val;}
  //----------------------------------------------------------------------
  float* getPointerYV(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char)+
                                       sizeof(float)+sizeof(float)+sizeof(float)+
                                       sizeof(float));}
  float getYV(int i){return *getPointerYV(i);}
  void setYV(int i,float val){*getPointerYV(i) = val;}
  //----------------------------------------------------------------------
  float* getPointerZV(int i)
  {return (float*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefTank+sizeof(unsigned char)+
                                       sizeof(float)+sizeof(float)+sizeof(float)+
                                       sizeof(float)+sizeof(float));}
  float getZV(int i){return *getPointerZV(i);}
  void setZV(int i,float val){*getPointerZV(i) = val;}
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_SCORE
class TA_Score : public TBasePacket
{
  // ushort type
  // uchar score0 
  // uchar score1
  // uint time_rest мс

public:
  TA_Score()
  {
    mType = APPL_TYPE_G_A_SCORE;
    mSize = sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned int);
    mData = (char*)malloc(mSize);
    setType();
  }
  //----------------------------------------------------------------------
  void setScore0(unsigned char val){*getPointerScore0()=val;}
  unsigned char  getScore0(){return *getPointerScore0();}
  unsigned char* getPointerScore0(){return (unsigned char*)(mData+sizeof(mType));}
  //----------------------------------------------------------------------
  void setScore1(unsigned char val){*getPointerScore1()=val;}
  unsigned char  getScore1(){return *getPointerScore1();}
  unsigned char* getPointerScore1(){return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char));}
  //----------------------------------------------------------------------
  void setTimeRest(unsigned int val){*getPointerTimeRest()=val;}
  unsigned int  getTimeRest(){return *getPointerTimeRest();}
  unsigned int* getPointerTimeRest()
  {return (unsigned int*)(mData+sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char));}
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_EVENT_IN_FIGHT
class TA_Event_In_Fight : public TBasePacket
{
  //### в будущем будет меняться

  // ushort type
  // uchar  cnt
    // ushort id_obj
    // uint   state

public:
  enum{
    eSizeDefEvent = sizeof(unsigned short)+sizeof(unsigned int),
  };

  TA_Event_In_Fight()
  {
    mType = APPL_TYPE_G_A_EVENT_IN_FIGHT;
    mSize = sizeof(mType)+sizeof(unsigned char);
    mData = (char*)malloc(mSize);
    setType();
  }
  //-----------------------------------------------------------------------------
  void setCnt(unsigned char cnt)
  {
    int new_size = sizeof(mType)+sizeof(unsigned char)+cnt*eSizeDefEvent;
    mData = (char*)mo_realloc(mData,mSize,new_size);
    mSize = new_size;

    unsigned char* pCnt = getPointerCnt();
    *pCnt = cnt;
  }
  unsigned char getCnt(){return *getPointerCnt();}
  unsigned char* getPointerCnt(){return (unsigned char*)(mData+sizeof(mType));}
  //-----------------------------------------------------------------------------
  void setID(int i, unsigned short id){*getPointerID(i)=id;}
  unsigned short getID(int i){return *getPointerID(i);}
  unsigned short* getPointerID(int i)
  {return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefEvent);}
  //-----------------------------------------------------------------------------
  void setState(int i, unsigned short state){*getPointerID(i)=state;}
  unsigned short getState(int i){return *getPointerState(i);}
  unsigned short* getPointerState(int i)
  {return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char)+i*eSizeDefEvent+sizeof(unsigned short));}
  //-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_EXIT_FIGHT
class TA_Exit_Fight : public TBasePacket
{
  // ushort type

public:
  TA_Exit_Fight()
  {
    mType = APPL_TYPE_A_EXIT_FIGHT;
    mSize = sizeof(mType);
    mData = (char*)malloc(mSize);
    setType();
  }
};
//-----------------------------------------------------------------------------
//APPL_TYPE_A_END_FIGHT
class TA_End_Fight : public TBasePacket
{
  // ushort type
  // uchar  code_exit
  // uchar  code_action выйти ли из боя ManagerGui
  // ushort size_msg длина сообщения

  // msg

public:

  enum{eWin=0,
       eLoss=1,
       eNoOnes=2};
  
  enum{eExitFalse=0,
       eExitTrue=1,
  };
  TA_End_Fight()
  {
    mType = APPL_TYPE_A_END_FIGHT;
    mSize = sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned short);
    mData = (char*)malloc(mSize);
    setType();
  }

  void setCodeExit(unsigned char code){*getPointerCodeExit()=code;}
  unsigned char getCodeExit(){return *getPointerCodeExit();}
  unsigned char* getPointerCodeExit(){return (unsigned char*)(mData+sizeof(mType));}
  //-------------------------------------------------------------------------------------
  void setCodeAction(unsigned char code){*getPointerCodeAction()=code;}
  unsigned char getCodeAction(){return *getPointerCodeAction();}
  unsigned char* getPointerCodeAction(){return (unsigned char*)(mData+sizeof(mType)+sizeof(unsigned char));}
  //-------------------------------------------------------------------------------------
  unsigned short getSizeMsg(){return *getPointerSizeMsg();}
protected:
  void setSizeMsg(unsigned short size){*getPointerSizeMsg()=size;}
  unsigned short* getPointerSizeMsg(){ return (unsigned short*)(mData+sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char));}

  char* getPointerStrMsg(){return mData+sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned short);}
public:
  //-------------------------------------------------------------------------------------
  void setMsg(char* sMsg)
  {
    unsigned short sizeMsg = strlen(sMsg)+1;
    int new_size = sizeof(mType)+sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned short)+sizeMsg;
    mData = (char*)mo_realloc(mData,mSize,new_size);
    mSize = new_size;
    char* sPointerMsg = getPointerStrMsg();
    strcpy(sPointerMsg,sMsg);

    setSizeMsg(sizeMsg);
  }
  void getMsg(char* sMsg)
  {
    char* sPointerMsg = getPointerStrMsg();
    strcpy(sMsg,sPointerMsg);
  }
  //-------------------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------

#endif