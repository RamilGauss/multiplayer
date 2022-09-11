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

#include "CmdParam.h"
 
class TInputCmdTornado
{
  
 TCmdParam mCmdParam;

 typedef std::vector<std::string> TVectorStr;
 TVectorStr mVecDefKey;

public:

	struct TInput
	{
		std::string libName;
		std::string param;
		std::string type; 
		int         variant_use;
    bool        useConsole;
		TInput()
		{
			variant_use = 0;
      useConsole  = false;
		}
	};

  TInputCmdTornado();
  ~TInputCmdTornado();

	bool SetArg(std::vector<std::string>& vecArgv);
	void Get(TInput& v_out);

protected:
	TInput mInput;
};