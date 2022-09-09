/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef ManagerStateMachine_H
#define ManagerStateMachine_H

#include "IManagerStateMachine.h"
#include "StateMachine.h"
#include <vector>

class IXML;

class TManagerStateMachine : public IManagerStateMachine
{
  struct TDesc
  {
    unsigned int v;
    std::string  comment;
    TDesc(unsigned int _v,std::string& _comment)
    {
      v = _v;
      comment = _comment;
    }
  };

  typedef std::map<unsigned int, TDesc> TMapUintPtr;

  typedef TStateMachine<unsigned int, TMapUintPtr> TSM_UintUint;

  typedef std::vector<TSM_UintUint*> TVectorPtr;

  TVectorPtr mVecSM;

  IXML* pXML;

public:

  TManagerStateMachine();
  virtual ~TManagerStateMachine();

  virtual bool Load(const char* sPath, unsigned int &id);
  virtual bool SetState(const char* sNewStateName, unsigned int id = eAll);
  virtual bool GetValue(unsigned int id, unsigned int k, unsigned int& v);
  virtual bool GetComment(unsigned int id, unsigned int k, std::string&  comment);

protected:

  void Done();

};


#endif