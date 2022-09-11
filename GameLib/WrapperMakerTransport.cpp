/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "WrapperMakerTransport.h"
#include "INetTransport.h"

using namespace nsMelissa;

TWrapperMakerTransport::TWrapperMakerTransport()
{

}
//-------------------------------------------------------------------------
TWrapperMakerTransport::~TWrapperMakerTransport()
{

}
//-------------------------------------------------------------------------
INetTransport* TWrapperMakerTransport::New()
{
  // ������� ��������� ���������� � ��� ���������
	return maker.New();
}
//-------------------------------------------------------------------------
void TWrapperMakerTransport::Delete(INetTransport* pTransport)
{
  maker.Delete(pTransport);
}
//-------------------------------------------------------------------------

