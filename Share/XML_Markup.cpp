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
the "Tanks" Source Code.  If not, please request a copy in writing at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "XML_Markup.h"

using namespace std;

bool TXML_Markup::Load(const char* sPath)
{
  mStrPath.insert(0, sPath );
  return mMarkup.Load(sPath);
}
//------------------------------------------------------------------
bool TXML_Markup::IsOpen()
{
  return mMarkup.IsWellFormed();
}
//------------------------------------------------------------------
// �������� � ��������
int TXML_Markup::GetCountSection(const char* name)
{
  mMarkup.SavePos();

  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    if(name==NULL)
      cntElem++;
    else
    {
      string sName = mMarkup.GetTagName();
      if(strcmp(name,sName.data())==0)
        cntElem++;
    }
  }
  mMarkup.RestorePos();
  return cntElem;
}
//------------------------------------------------------------------
// ���������
bool TXML_Markup::EnterSection(const char* name, int num)
{
  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    if(name==NULL)
      cntElem++;
    else
    {
      string sName = mMarkup.GetTagName();
      if(strcmp(name,sName.data())==0)
      {
        if(num==cntElem)
          return mMarkup.IntoElem();
        cntElem++;
      }
    }
  }
  return false;
}
//------------------------------------------------------------------
bool TXML_Markup::LeaveSection()
{
  if(mMarkup.OutOfElem())
  {
    mMarkup.ResetMainPos();
    return true;
  }
  return false;
}
//------------------------------------------------------------------
// ��������� ���-��
bool TXML_Markup::AddSection(const char* name)
{
  mMarkup.SavePos();
    bool res = mMarkup.AddElem(name, " ");
  mMarkup.RestorePos();
  return res;
}
//------------------------------------------------------------------
bool TXML_Markup::RemoveSection(const char* name, int num)
{
  bool res = false;
  mMarkup.SavePos();
    
  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    string sName = mMarkup.GetTagName();
    if(strcmp(name,sName.data())==0)
    {
      if(num==cntElem)
      {
        res = mMarkup.RemoveElem();
        break;
      }
      cntElem++;
    }
  }

  mMarkup.RestorePos();
  return res;
}
//------------------------------------------------------------------
bool TXML_Markup::WriteSection(const char* name, int num, std::string buffer)
{
  mMarkup.SavePos();
  bool res = false;
  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    string sName = mMarkup.GetTagName();
    if(strcmp(name,sName.data())==0)
    {
      if(num==cntElem)
      {
        res = mMarkup.SetElemContent(buffer.data());
        break;
      }
      cntElem++;
    }
  }
  mMarkup.RestorePos();
  return res;
}
//------------------------------------------------------------------
// ������
std::string TXML_Markup::ReadSection(const char* name, int num)
{
  string content;
  mMarkup.SavePos();

  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    string sName = mMarkup.GetTagName();
    if(strcmp(name,sName.data())==0)
    {
      if(num==cntElem)
      {
        content = mMarkup.GetElemContent();
        break;
      }
      cntElem++;
    }
  }

  mMarkup.RestorePos();
  return content;
}
//------------------------------------------------------------------
string TXML_Markup::GetNameSection(int num)
{
  string name;
  mMarkup.SavePos();

  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    if(num==cntElem)
    {
      name = mMarkup.GetTagName();
      break;
    }
    cntElem++;
  }
  mMarkup.RestorePos();
  return name;
}
//------------------------------------------------------------------
bool TXML_Markup::Save(const char* sPath)
{
  if(sPath==NULL)
    sPath = mStrPath.data();
  return mMarkup.Save(sPath);
}
//------------------------------------------------------------------
void TXML_Markup::ResetPos()
{
  mMarkup.ResetPos();
}
//------------------------------------------------------------------
bool TXML_Markup::WriteSectionAttr(const char* name, int num, const char* nameAttr, std::string buffer)
{
  mMarkup.SavePos();
  bool res = false;
  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    string sName = mMarkup.GetTagName();
    if(strcmp(name,sName.data())==0)
    {
      if(num==cntElem)
      {
        res = mMarkup.SetAttrib(nameAttr,buffer.data());
        break;
      }
      cntElem++;
    }
  }
  mMarkup.RestorePos();
  return res;
}
//------------------------------------------------------------------
// ������
std::string TXML_Markup::ReadSectionAttr(const char* name, int num, const char* nameAttr)
{
  string content;
  mMarkup.SavePos();

  int cntElem = 0;
  while(mMarkup.FindElem())
  { 
    string sName = mMarkup.GetTagName();
    if(strcmp(name,sName.data())==0)
    {
      if(num==cntElem)
      {
        content = mMarkup.GetAttrib(nameAttr);
        break;
      }
      cntElem++;
    }
  }

  mMarkup.RestorePos();
  return content;
}
//------------------------------------------------------------------
bool TXML_Markup::AddXMLDeclaration( const char* strDecl )
{
  return mMarkup.AddNode(CMarkup::MNT_PROCESSING_INSTRUCTION, strDecl);
}
//------------------------------------------------------------------