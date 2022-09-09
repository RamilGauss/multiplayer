/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ShaderStackH
#define ShaderStackH

#include <string>
#include <vector>

class TShaderStack
{

  struct TData
  {
    std::string name;
    char* data;
    int size;
    TData(){data=NULL;size=0;}
    ~TData(){Done();}
    void Done(){delete[]data;data=NULL;size=0;}
    void Set(char* p,int s)
    {
      if(s!=size)
      {
        Done();
        size=s;
        data=new char[s];
      }
      memcpy(data,p,size);
    }
  };
  
  std::vector<TData*> mVecorNameData;

public:
  TShaderStack();
  ~TShaderStack();

  int Count(){return mVecorNameData.size();}
  int Push(const char* nameValueIn, void* pDataIn, int sizeIn);

  int GetIndexByName(const char* name);

  void SetData(int index, void* pDataIn, int sizeIn);

  std::string GetName(int index);
  void* GetData(int index, int& size);

  void Done();

};

#endif