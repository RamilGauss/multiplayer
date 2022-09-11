/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "InputCmdTornado.h"

#define KEY_PARAM   string("-p")
#define KEY_LIB     string("-d")
#define KEY_VARIANT string("-v")
#define KEY_REALIZE string("-r")

using namespace std;

TInputCmdTornado::TInputCmdTornado()
{
	mVecDefKey.push_back(KEY_VARIANT);
	mVecDefKey.push_back(KEY_LIB    );
	mVecDefKey.push_back(KEY_REALIZE);
	mVecDefKey.push_back(KEY_PARAM  );

	mCmdParam.SetDefKey(mVecDefKey);
}
//-------------------------------------------------------------------------------
TInputCmdTornado::~TInputCmdTornado()
{

}
//-------------------------------------------------------------------------------
bool TInputCmdTornado::SetArg(vector<string>& vecArgv)
{
	mCmdParam.SetArg(vecArgv);

	int cD = mCmdParam.GetCountValueByKey(KEY_LIB);
	if(cD==0) return false;
	mCmdParam.GetByKey(KEY_LIB, 0, mInput.libName);
	//-------------------------------------------------
	int cR = mCmdParam.GetCountValueByKey(KEY_REALIZE);
	if(cR==0) return false;
	mCmdParam.GetByKey(KEY_REALIZE, 0, mInput.type);
	//-------------------------------------------------
	int cV = mCmdParam.GetCountValueByKey(KEY_VARIANT);
	if(cV==1)
	{
		string sVariant;
		mCmdParam.GetByKey(KEY_VARIANT, 0, sVariant);
		mInput.variant_use = atoi(sVariant.data());
	}
	//-------------------------------------------------
	int cP = mCmdParam.GetCountValueByKey(KEY_PARAM);
	if(cP==1)
		mCmdParam.GetByKey(KEY_PARAM, 0, mInput.param);

	return true;
}
//-------------------------------------------------------------------------------
void TInputCmdTornado::Get(TInputCmdTornado::TInput& v_out)
{
	v_out = mInput;
}
//-------------------------------------------------------------------------------
