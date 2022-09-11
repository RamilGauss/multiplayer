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
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#include "AutomatRegularSpeed.h"
#include "DoserProtocolPacket.h"
#include "BreakPacket.h"
#include "INetTransport.h"

using namespace nsNetDoser;

TAutomatRegularSpeed::TAutomatRegularSpeed(TIP_Port* ip_port)
{
  mIP_Port = *ip_port;
  mTransport = NULL;
}
//------------------------------------------------------------------------
TAutomatRegularSpeed::~TAutomatRegularSpeed()
{

}
//------------------------------------------------------------------------
bool TAutomatRegularSpeed::AddInQueue(int sizeBuffer)// ��������� �� � ������� ������, ����� ������ ������� sizeBuffer
{
  return mStateARS.TrySendWithoutBuffer(sizeBuffer);
}
//------------------------------------------------------------------------
void TAutomatRegularSpeed::Add(TDescSendPacket* pDescPacket)// �������� 
{
  mStateARS.UpSize(pDescPacket->GetPacket()->GetSize());// �������� � ����� ������

  mListPacket.push_back(TReadyPacket());
  TReadyPacket& rp = mListPacket.back();
  rp.d = *pDescPacket;
  rp.Init();
}
//------------------------------------------------------------------------
bool TAutomatRegularSpeed::NeedSend()// �����, ���� �� ���-�� ����������
{
  // ������� �������
  if(mListPacket.size()==0) return false;
  return true;
}
//------------------------------------------------------------------------
// ����� �������� �� ��� ��� ���� �� ������� ����� ��� �� ���������� ������
void TAutomatRegularSpeed::Send()
{
  TListReadyPacketIt bit = mListPacket.begin();
  TListReadyPacketIt eit = mListPacket.end();
  while(bit!=eit)
  {
    int size = bit->d.GetPacket()->GetSize();
    if(mStateARS.TrySendFromBuffer(size))
    {
      // ����� TReadyPacket::Init() �������������� ���������� 1 �����
      void* p = bit->d.GetPacket()->GetCollectPtr();
     ((THeaderSinglePacket*)p)->SetTime();// ��������� ����� �����������

      mTransport->Send(mIP_Port.ip, mIP_Port.port, *(bit->d.GetPacket()), bit->d.GetCheck());
      mStateARS.DownSize(size);
      mListPacket.erase(bit);
      bit = mListPacket.begin();
    }
    else
      return;
  }
}
//------------------------------------------------------------------------
void TAutomatRegularSpeed::Overload(unsigned char cntTry)// �������� ����� � �������
{
	mStateARS.Event(cntTry);
}
//------------------------------------------------------------------------
void TAutomatRegularSpeed::SetTransport(INetTransport* pTransport)
{
  mTransport = pTransport;
}
//------------------------------------------------------------------------
void TAutomatRegularSpeed::TReadyPacket::Init()
{
  d.GetPacket()->Collect();// ������� �����
  // ������ �� ��� �������
  char* p = (char*)d.GetPacket()->GetCollectPtr();
  int size = d.GetPacket()->GetSize();
  if(d.GetPacket()->GetCountPart()==1)
  {
    // ���� ���-�� ������ ����� 1, �� ����������� �� ���������� � ������ ��� � ����� ����������
    // �������, ����� ����� ����������� � ���� �����
    collectPacket.SetData(p, size);
    p = (char*)collectPacket.GetPtr();// ��� ��������� � ������ BreakPacket
  }

  d.GetPacket()->UnlinkPart();
  d.GetPacket()->PushBack(p,size);
}
//------------------------------------------------------------------------
