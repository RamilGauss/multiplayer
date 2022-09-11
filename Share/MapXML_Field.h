/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MapXML_FieldH
#define MapXML_FieldH

#include <map>
#include <string>
#include "IXML.h"
#include "TypeDef.h"

#ifdef WIN32
#pragma warning(disable:4251)
#endif

class SHARE_EI TMapXML_Field
{
  typedef std::multimap<std::string,std::string> TMultiMapStrStr;

  TMultiMapStrStr mMap;

  IXML* mXML;
    
public:

  TMapXML_Field();
  virtual ~TMapXML_Field();
  
  // ��������� XML
  bool Load(const char* sAbsPath);

  // ������� �� ����� �������� 
	const char* GetSecond(const char* name, unsigned int index = 0);
  unsigned int GetCount(const char* name);

protected:
};

#endif