/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "HistoryPacketTCP.h"
#include "HeaderTCP.h"
#include "INetTransport.h"
#include "Logger.h"


THistoryPacketTCP::THistoryPacketTCP()
{
  state = eSearchBegin;
  sizePacket = 0;
}
//------------------------------------------------------------------------
void THistoryPacketTCP::Clear()
{
  state      = eSearchBegin;
  sizePacket = 0;
}
//------------------------------------------------------------------------
void THistoryPacketTCP::Analiz(int& beginPos, TResult& res, 
                               int readSize, char* buffer)
{
  switch(state)
  {
    case eSearchBegin:
      beginPos += SearchBegin(readSize,buffer,beginPos);
      break;
    case eSearchSize:
      beginPos += SearchSize(readSize,buffer,beginPos);
      break;
    case eSearchEnd:
      beginPos += SearchEnd(readSize,buffer,res,beginPos);
      break;
  }
}
//----------------------------------------------------------------------------------
int THistoryPacketTCP::SearchBegin(int readSize, char* buffer, int beginPos)
{
  if( readSize - beginPos < sizeof(THeaderTCP) )
  {
    state = eSearchSize;
    c.SetData(&buffer[beginPos], readSize - beginPos);
    return readSize - beginPos;// вернуть сколько истратили
  }
  // ищем в буфере начало
  THeaderTCP header;
  THeaderTCP* pHeader = (THeaderTCP*)&buffer[beginPos];
  if(pHeader->header!=header.header)
  {
    GetLogger(STR_NAME_NET_TRANSPORT)->
      WriteF_time("THistoryPacketTCP::SearchBegin FAIL.\n");
  }

  state      = THistoryPacketTCP::eSearchEnd;
  sizePacket = pHeader->size;

  c.SetData(&buffer[beginPos], sizeof(THeaderTCP));
  return sizeof(THeaderTCP);// вернуть сколько истратили
}
//----------------------------------------------------------------------------------
int THistoryPacketTCP::SearchSize(int readSize, char* buffer, int beginPos)
{
  if(readSize - beginPos + c.GetSize() < sizeof(THeaderTCP))
  {
    // так и не нашли заголовок полностью
    c.AddData(&buffer[beginPos], readSize - beginPos);
    return readSize - beginPos;// вернуть сколько истратили
  }
  int needCopy = sizeof(THeaderTCP) - c.GetSize();
  c.AddData(&buffer[beginPos], needCopy);

  THeaderTCP header;
  THeaderTCP* pHeader = (THeaderTCP*)c.GetPtr();
  if(pHeader->header!=header.header)
  {
    GetLogger(STR_NAME_NET_TRANSPORT)->
      WriteF_time("THistoryPacketTCP::SearchSize FAIL.\n");
  }

  state      = THistoryPacketTCP::eSearchEnd;
  sizePacket = pHeader->size;
  return needCopy;// вернуть сколько истратили
}
//----------------------------------------------------------------------------------
int THistoryPacketTCP::SearchEnd(int readSize, char* buffer,
                                 TResult& res, int beginPos)
{
  int mustSize = sizePacket + sizeof(THeaderTCP);// предполагаемый размер
  // не хватает до полного размера
  if( mustSize > readSize - beginPos + c.GetSize()/*остаток внутри истории*/)
  {
    // скопировать внутрь и выйти
    c.AddData(&buffer[beginPos], readSize - beginPos);
    return readSize - beginPos;
  }
  if( mustSize == readSize - beginPos + c.GetSize())// полный пакет
  {
    c.AddData(&buffer[beginPos], readSize - beginPos);

    res.Set(sizeof(THeaderTCP)+c.GetPtr(),sizePacket);
    Clear();
    return readSize - beginPos;
  }
  // пакет собран
  int needSize = mustSize - c.GetSize();// не хватает до полного пакета
  c.AddData(&buffer[beginPos], needSize );

  res.Set(sizeof(THeaderTCP)+c.GetPtr(),sizePacket);
  Clear();
  return needSize;
}
//----------------------------------------------------------------------------------
