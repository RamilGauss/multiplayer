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

