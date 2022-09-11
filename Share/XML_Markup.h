/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef XML_MarkupH
#define XML_MarkupH

#include "IXML.h"
#include "Markup.h"

#define A2UTF8  CMarkup::AToUTF8
#define UTF82A  CMarkup::UTF8ToA

/*
  ���������� XML-������� �� CMarkUp.
*/

class TXML_Markup : public IXML
{
protected:
  CMarkup mMarkup;
  std::string mStrPath;

public:
  TXML_Markup(){};
  virtual ~TXML_Markup(){};

  virtual bool Load(const char* sPath);
  virtual bool IsOpen();
  // �������� � ��������
  
  // ���������
  virtual void ResetPos();
  virtual int  GetCountSection(const char* name = NULL);
  virtual std::string GetNameSection(int index);
  virtual bool EnterSection(const char* name, int num);
  virtual bool LeaveSection();
  // ��������� ���-��
  virtual bool AddSectionAndEnter(const char* name);
  virtual bool AddSectionAndEnter(const char *name, int numAttr, TAttrInfo *pAttribs);
  virtual bool AddSection(const char* name);
  virtual bool AddSection(const char *name, int numAttr, TAttrInfo *pAttribs);
  virtual bool RemoveSection(const char* name, int num);

  // ��������� �����������
  virtual bool AddXMLDeclaration(const char* strDecl);

  virtual bool WriteSectionAttr(const char* name, int num, const char* nameAttr, std::string buffer);
  virtual bool WriteSection(const char* name, int num, std::string buffer);
  
  virtual bool WriteSectionAttr(int index, const char* nameAttr, std::string buffer);
  virtual bool WriteSection(int index, std::string buffer);
  // ������
  virtual std::string ReadSectionAttr(const char* name, int num, const char* nameAttr );
  virtual std::string ReadSection(const char* name, int num);
  
  virtual std::string ReadSectionAttr(int index, const char* nameAttr );
  virtual std::string ReadSection(int index);

  virtual bool Save(const char* sPath = NULL);
};

#endif