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

