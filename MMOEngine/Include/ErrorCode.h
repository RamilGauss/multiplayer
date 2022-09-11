/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

namespace nsMMOEngine
{
  typedef enum
  {
    // LoginSlave
    LoginSlave_MasterNotReady,
    LoginSlave_NoAnswerFromMaster,
    //--------------------------------------
    // LoginMaster
    LoginMaster_SSNotReady,
    LoginMaster_NoAnswerFromSS,
    //--------------------------------------
    // LoginClient
    LoginClientClientMasterNotReady,
    LoginClientClientNoAnswer,
    LoginClientClientNotExistSlave,
    LoginClientSlaveNoAnswer,
    LoginClientMasterClientNotActive,
    LoginClientSuperServerNoAnswer,
  }ErrorCode;
}
#endif