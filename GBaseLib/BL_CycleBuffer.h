/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_CycleBufferH
#define BL_CycleBufferH

//-----------------------------------------------------------------------------
template< int sizeBuffer >  // размер буфера, должен быть степенью 2
struct TCycleBufferT         // Циклический буфер
{
  unsigned char data[sizeBuffer];  // буфер
  int begin;                // индех первого занятого элемента
  int end;                  // индех первого свободного элемента
  
	TCycleBufferT<sizeBuffer >() { begin = end = 0; }

  // Занятый объем буфера
  int Size()            { return (end - begin) & (sizeBuffer-1); }
  
  // буфер пуст
  bool Empty()          { return end == begin; }

  // очистить буфер
  void Clear()          { begin = end = 0; }
  //.......................................................................
  //......     Операции с указателями головы и хвоста буфера   ............
  //.......................................................................

  // вычислить индекс следующего элемента буфера
  int Next( int index ) { return (index+1) & (sizeBuffer-1); }
  
  // вычислить индекс предыдущего элемента буфера
  int Prev( int index ) { return (index-1) & (sizeBuffer-1); }
  
  // вычислить индекс смещенного на <step> элемента буфера
  int Skip( int index, int step ) { return (index+step) & (sizeBuffer-1); }

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для записи размером не превышающим <size>
  // Результат: указатель на блок и его размер
  unsigned char * GetBlockForWrite( int &size ) // бывшая GetFlatBlock
  {
    int sizeBlock =( end >= begin ) ? sizeBuffer - end : begin - end;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + end;
  }

  // Отметить, что в буфер записано <size> байт
	void MarkWrited( int size ) { end = Skip( end, size); }

  unsigned char* GetFlatBlock( int &size ) { return GetBlockForWrite(size); }// устаревшая, для совместимости

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для чтения размером не превышающим <size>
  // Результат: указатель на блок и его размер
  unsigned char* GetBlockForRead( int &size ) 
  {
    if ( begin == end )
		{
			size = 0;
			return NULL;
		}
		int sizeBlock = ( begin <= end ) ? end - begin : sizeBuffer - begin;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + begin;
  }

  // Отметить, что из буфера считано <size> байт
  void MarkReaded( int size ) { begin = Skip( begin, size ); }
  //.......................................................................
};
//-----------------------------------------------------------------------------
template<>
struct TCycleBufferT<256>    // Циклический буфер на 256 значений
{
  enum { sizeBuffer = 256 };
  guint8 data[sizeBuffer];  // буфер
  guint8 begin;             // индех первого занятого элемента
  guint8 end;               // индех первого свободного элемента
  
	TCycleBufferT<256>() { begin = end = 0; }

  // Занятый объем буфера
  int Size()            { return (guint8)(end - begin); }

  // буфер пуст
  bool Empty()          { return end == begin; }

  // очистить буфер
  void Clear()          { begin = end = 0; }
  //.......................................................................
  //......     Операции с указателями головы и хвоста буфера   ............
  //.......................................................................

  // вычислить индекс следующего элемента буфера
  int Next( int index ) { return (guint8)(index+1); }
  
  // вычислить индекс предыдущего элемента буфера
  int Prev( int index ) { return (guint8)(index-1); }
  
  // вычислить индекс смещенного на <step> элемента буфера
  int Skip( int index, int step ) { return (guint8)(index+step); }

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для записи размером не превышающим <size>
  // Результат: указатель на блок и его размер
  unsigned char* GetBlockForWrite( int &size ) // бывшая GetFlatBlock
  {
    int sizeBlock =( end >= begin ) ? sizeBuffer - end : begin - end;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + end;
  }

  // Отметить, что в буфер записано <size> байт
  void MarkWrited( int size ) { end = (guint8)Skip( end, size); }

  unsigned char * GetFlatBlock( int &size ) { return GetBlockForWrite(size); }// устаревшая, для совместимости

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для чтения размером не превышающим <size>
  // Результат: указатель на блок и его размер
  guint8* GetBlockForRead( int &size ) 
  {
    int sizeBlock = ( begin <= end ) ? end - begin : sizeBuffer - begin;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + begin;
  }

  // Отметить, что из буфера считано <size> байт
  void MarkReaded( int size ) { begin = (guint8)Skip( begin, size ); }

  //.......................................................................
};

typedef struct TCycleBufferT<256>  TCycleBuffer256;

//-----------------------------------------------------------------------------

class TCycleBuffer         // Циклический буфер общего назначения
{
  char* mData;      // буфер
  unsigned int mSize;                 // размер буфера
  unsigned int mCntWrited;             // объем данных записанный в буфер
  unsigned int mCntReaded;              // объем данных вычитанный из буфера

public:
  TCycleBuffer() { mData = NULL; mSize = mCntWrited = mCntReaded = 0; }
  
  TCycleBuffer( int size ) 
  { 
    mData = new char[size]; 
    mSize = size;
    mCntWrited = mCntReaded = 0;
  }
  
  void done() 
  { 
    Clear();
    delete[] mData; mData = NULL; 
  }

  void init( unsigned int size ) 
  { 
    done();
    mData = new char[size]; mSize = size;
  }

  // Занятый объем буфера
  unsigned int Size() const  { return mCntWrited - mCntReaded; }

  // Пустой объем буфера
  unsigned int EmptySize() const  { return mSize - (mCntWrited - mCntReaded); }

  // буфер пуст
  bool isEmpty() const       { return mCntWrited == mCntReaded; }

  // буфер полон
  bool isFull() const        { return mCntWrited - mCntReaded >= mSize; }

  // очистить буфер
  void Clear()                { mCntWrited = mCntReaded = 0; }

  // объем данных записанный в буфер
  unsigned int cntWrited() const { return mCntWrited; }
  
  // объем данных вычитанный из буфера
  unsigned int cntReaded() const { return mCntReaded; }              

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для записи размером не превышающим <size>
  // Результат: указатель на блок и его размер
  char * GetBlockForWrite( unsigned int &size ) const
  {
    if( isFull() ) { size = 0; return NULL; }
    unsigned int posW = mCntWrited % mSize;
    unsigned int posR = mCntReaded % mSize;
    unsigned int hasPlace = ( posW < posR ) ? posR-posW : mSize-posW;
    if( size > hasPlace ) size = hasPlace;
    return mData + posW;
  }

  // Отметить, что в буфер записано <size> байт
  void MarkWrited( unsigned int size ) { mCntWrited += size; }

  //.......................................................................
  //..........     Поддержка для блочной записи в буфер   .................
  //.......................................................................

  // Предоставить участок буфера для чтения размером не превышающим <size>
  // Результат: указатель на блок и его размер
  char* GetBlockForRead( unsigned int &size ) const
  {
    if( isEmpty() ) { size = 0; return NULL; }
    unsigned int posR = mCntReaded % mSize;
    unsigned int posW = mCntWrited % mSize;
    unsigned int hasData = ( posR < posW ) ? posW-posR : mSize-posR;
    if( size > hasData ) size = hasData;
    return mData + posR;
  }

  // Отметить, что из буфера считано <size> байт
  void MarkReaded( unsigned int size ) { mCntReaded += size; }
  //.......................................................................
};
//-----------------------------------------------------------------------------

#endif

