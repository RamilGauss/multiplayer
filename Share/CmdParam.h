/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include <map>
#include <string>
#include <vector>
#include <set>

#include "TypeDef.h"

class SHARE_EI TCmdParam
{
  typedef std::multimap<std::string,std::string> TMultiMapStrStr;
  typedef TMultiMapStrStr::iterator TMultiMapStrStrIt;

  typedef std::set<std::string> TSetStr;
  typedef TSetStr::iterator TSetStrIt;

  TMultiMapStrStr mMapKey;
  TSetStr mSetDefKey;
  TSetStr mSetFoundKey;
public:
  TCmdParam();
  virtual ~TCmdParam();
  // ������ ������ ��������� ������
  void SetDefKey(std::vector<std::string>& vecKey);
  // ������� ��������� ��������� ������, ������� �� �������� ������
  void SetArg(std::vector<std::string>& vecArgv);
  // ���� �� ����� ����?
  bool IsKey(std::string& sKey);
  // ���������� ���-�� ��������, ������ ����� ����� � ������ ���������� 
  // �� ���������� ����� ��� �� ��������� ������
  int  GetCountValueByKey(std::string& sKey);
  // �������� ������ �� �����
  bool GetByKey(std::string& sKey, int index, std::string& sOut);
protected:
  bool FindInDefKey( std::string& sKey );
};