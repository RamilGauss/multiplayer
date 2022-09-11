/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

namespace nsMMOEngine
{
  typedef enum
  {
		// open
		OpenClient_MoreThenOneSubNet,
    // CreateGroup
    CreateGroup_ClientNotExist,
    // LoginSlave
    LoginSlave_MasterNotReady,
    LoginSlave_NoAnswerFromMaster,
    //--------------------------------------
    // LoginMaster
    LoginClientMaster_KeyBusy,
    LoginMaster_SSNotReady,
    LoginMaster_NoAnswerFromSS,
    //--------------------------------------
    // LoginClient
    LoginClientClientSecondEnter,
    LoginClientClientMasterNotReady,
    LoginClientClientNoAnswer,
    LoginClientClientNotExistSlave,
    LoginClientSlaveNoAnswer,
    LoginClientMasterClientNotActive,
    LoginClientSuperServerNoAnswer,
  }ErrorCode;
}
#endif