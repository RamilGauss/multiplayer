/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#include "IXML.h"

using namespace std;


bool IXML::AddChildSection(const char* childName, int num, const char* name)
{
  if(EnterSection(childName, num)==false) return false;
    if(AddSection(name)==false) return false;
  if(LeaveSection()==false) return false;
  return true;
}
//------------------------------------------------------------------
bool IXML::RemoveChildSection(const char* childName, int numChild, const char* name, int num)
{
  if(EnterSection(childName, numChild)==false) return false;
    if(RemoveSection(name,num)==false) return false;
  if(LeaveSection()==false) return false;
  return true;
}
//------------------------------------------------------------------
bool IXML::ReadFloat(const char* name, int num, float & v)
{
  string s = ReadSection(name, num);
  if(s.length()==0)
    return false;
  v = (float)atof(s.data());
  return true;
}
//------------------------------------------------------------------
bool IXML::ReadInt(const char* name, int num, int & v)
{
  string s = ReadSection(name, num);
  if(s.length()==0)
    return false;
  v = atoi(s.data());
  return true;
}
//------------------------------------------------------------------
bool IXML::ReadUint(const char* name, int num, unsigned int & v)
{
  string s = ReadSection(name, num);
  if(s.length()==0)
    return false;
  v = atoi(s.data());
  return true;
}
//------------------------------------------------------------------
bool IXML::ReadFloat3(const char* name, int num, float * v3)// ����������� ��� "space" ��� ";"
{
  string s = ReadSection(name, num);
  int size = s.length();
  if(size==0)
    return false;
  if(sscanf(s.data(),"%f;%f;%f",&v3[0],&v3[1],&v3[2])!=3) return false;

  return true;
}
//------------------------------------------------------------------
bool IXML::ReadFloat4(const char* name, int num, float * v4)
{
  string s = ReadSection(name, num);
  int size = s.length();
  if(size==0)
    return false;
  if(sscanf(s.data(),"%f;%f;%f;%f",&v4[0],&v4[1],&v4[2],&v4[3])!=4) return false;

  return true;
}
//------------------------------------------------------------------
