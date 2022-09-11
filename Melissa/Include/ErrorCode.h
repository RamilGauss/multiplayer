/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_ERROR_CODE_H
#define MELISSA_ERROR_CODE_H

namespace nsMelissa
{
  typedef enum
  {
    LoginSlave_MasterNotReady,
    LoginSlave_NoAnswerFromMaster,
    LoginMaster_SSNotReady,
    LoginMaster_NoAnswerFromSS,
    LoginClient_MasterNotReady,
    
  }ErrorCode;
}
#endif