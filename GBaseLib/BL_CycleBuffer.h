/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
template< int sizeBuffer >  // ������ ������, ������ ���� �������� 2
struct TCycleBufferT         // ����������� �����
{
  unsigned char data[sizeBuffer];  // �����
  int begin;                // ����� ������� �������� ��������
  int end;                  // ����� ������� ���������� ��������
  
	TCycleBufferT<sizeBuffer >() { begin = end = 0; }

  // ������� ����� ������
  int Size()            { return (end - begin) & (sizeBuffer-1); }
  
  // ����� ����
  bool Empty()          { return end == begin; }

  // �������� �����
  void Clear()          { begin = end = 0; }
  //.......................................................................
  //......     �������� � ����������� ������ � ������ ������   ............
  //.......................................................................

  // ��������� ������ ���������� �������� ������
  int Next( int index ) { return (index+1) & (sizeBuffer-1); }
  
  // ��������� ������ ����������� �������� ������
  int Prev( int index ) { return (index-1) & (sizeBuffer-1); }
  
  // ��������� ������ ���������� �� <step> �������� ������
  int Skip( int index, int step ) { return (index+step) & (sizeBuffer-1); }

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
  unsigned char * GetBlockForWrite( int &size ) // ������ GetFlatBlock
  {
    int sizeBlock =( end >= begin ) ? sizeBuffer - end : begin - end;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + end;
  }

  // ��������, ��� � ����� �������� <size> ����
	void MarkWrited( int size ) { end = Skip( end, size); }

  unsigned char* GetFlatBlock( int &size ) { return GetBlockForWrite(size); }// ����������, ��� �������������

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
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

  // ��������, ��� �� ������ ������� <size> ����
  void MarkReaded( int size ) { begin = Skip( begin, size ); }
  //.......................................................................
};
//-----------------------------------------------------------------------------
template<>
struct TCycleBufferT<256>    // ����������� ����� �� 256 ��������
{
  enum { sizeBuffer = 256 };
  guint8 data[sizeBuffer];  // �����
  guint8 begin;             // ����� ������� �������� ��������
  guint8 end;               // ����� ������� ���������� ��������
  
	TCycleBufferT<256>() { begin = end = 0; }

  // ������� ����� ������
  int Size()            { return (guint8)(end - begin); }

  // ����� ����
  bool Empty()          { return end == begin; }

  // �������� �����
  void Clear()          { begin = end = 0; }
  //.......................................................................
  //......     �������� � ����������� ������ � ������ ������   ............
  //.......................................................................

  // ��������� ������ ���������� �������� ������
  int Next( int index ) { return (guint8)(index+1); }
  
  // ��������� ������ ����������� �������� ������
  int Prev( int index ) { return (guint8)(index-1); }
  
  // ��������� ������ ���������� �� <step> �������� ������
  int Skip( int index, int step ) { return (guint8)(index+step); }

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
  unsigned char* GetBlockForWrite( int &size ) // ������ GetFlatBlock
  {
    int sizeBlock =( end >= begin ) ? sizeBuffer - end : begin - end;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + end;
  }

  // ��������, ��� � ����� �������� <size> ����
  void MarkWrited( int size ) { end = (guint8)Skip( end, size); }

  unsigned char * GetFlatBlock( int &size ) { return GetBlockForWrite(size); }// ����������, ��� �������������

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
  guint8* GetBlockForRead( int &size ) 
  {
    int sizeBlock = ( begin <= end ) ? end - begin : sizeBuffer - begin;  
    if( size > sizeBlock ) size = sizeBlock;
    return data + begin;
  }

  // ��������, ��� �� ������ ������� <size> ����
  void MarkReaded( int size ) { begin = (guint8)Skip( begin, size ); }

  //.......................................................................
};

typedef struct TCycleBufferT<256>  TCycleBuffer256;

//-----------------------------------------------------------------------------

class TCycleBuffer         // ����������� ����� ������ ����������
{
  char* mData;      // �����
  unsigned int mSize;                 // ������ ������
  unsigned int mCntWrited;             // ����� ������ ���������� � �����
  unsigned int mCntReaded;              // ����� ������ ���������� �� ������

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

  // ������� ����� ������
  unsigned int Size() const  { return mCntWrited - mCntReaded; }

  // ������ ����� ������
  unsigned int EmptySize() const  { return mSize - (mCntWrited - mCntReaded); }

  // ����� ����
  bool isEmpty() const       { return mCntWrited == mCntReaded; }

  // ����� �����
  bool isFull() const        { return mCntWrited - mCntReaded >= mSize; }

  // �������� �����
  void Clear()                { mCntWrited = mCntReaded = 0; }

  // ����� ������ ���������� � �����
  unsigned int cntWrited() const { return mCntWrited; }
  
  // ����� ������ ���������� �� ������
  unsigned int cntReaded() const { return mCntReaded; }              

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
  char * GetBlockForWrite( unsigned int &size ) const
  {
    if( isFull() ) { size = 0; return NULL; }
    unsigned int posW = mCntWrited % mSize;
    unsigned int posR = mCntReaded % mSize;
    unsigned int hasPlace = ( posW < posR ) ? posR-posW : mSize-posW;
    if( size > hasPlace ) size = hasPlace;
    return mData + posW;
  }

  // ��������, ��� � ����� �������� <size> ����
  void MarkWrited( unsigned int size ) { mCntWrited += size; }

  //.......................................................................
  //..........     ��������� ��� ������� ������ � �����   .................
  //.......................................................................

  // ������������ ������� ������ ��� ������ �������� �� ����������� <size>
  // ���������: ��������� �� ���� � ��� ������
  char* GetBlockForRead( unsigned int &size ) const
  {
    if( isEmpty() ) { size = 0; return NULL; }
    unsigned int posR = mCntReaded % mSize;
    unsigned int posW = mCntWrited % mSize;
    unsigned int hasData = ( posR < posW ) ? posW-posR : mSize-posR;
    if( size > hasData ) size = hasData;
    return mData + posR;
  }

  // ��������, ��� �� ������ ������� <size> ����
  void MarkReaded( unsigned int size ) { mCntReaded += size; }
  //.......................................................................
};
//-----------------------------------------------------------------------------

#endif

