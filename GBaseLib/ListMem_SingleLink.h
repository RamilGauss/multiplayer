#ifndef ListMem_SingleLinkH
#define ListMem_SingleLinkH

#include "TypeDef.h"

//--------------------------------------------------------------------------
template< size_t sizeInfo >
class TSLL_ListMemT
{
protected:
  // заголовок блока элемента списка
  struct TBlock
  {
		TBlock        *pNext;
    int           size;
  }_PACKED;

  static size_t  sizeHeader();
  static char*   blockToData( void* block );
  static void*   dataToBlock( void* data );
protected:
	TBlock *pFirst;
	TBlock *pLast;
  int     mCnt;
  
  // ƒостать из списка первый блок
  void* removeFirstBlock();

public:
	TSLL_ListMemT();
	~TSLL_ListMemT(); 

  // ќсвободить пам€ть зан€тую пакетом
  static void free( void* data );

  // освобождение всей пам€ти
  void clear();

  // кол-во блоков
  int count()     { return mCnt; }
    
  // список пуст
  bool empty()    { return mCnt == 0; }

  // добавить пакет в конец списка 
  void append( void* data, int size );

  // ƒостать из списка первый пакет
	void* remoteFirst( int &size );

  // ¬ернуть в список первый пакет
  void returnFirst( void* data );

  // первый элемент списка
  void* first( int &size );

  // элемент списка, следующий за указанным <item>
  void* next( void* item, int &size );
};


typedef TSLL_ListMemT<0>    TSLL_ListMem;

//--------------------------------------------------------------------------

template< typename TInfo >
class TSLL_ListMemTI : public TSLL_ListMemT< sizeof(TInfo) >
{
  typedef TSLL_ListMemT< sizeof(TInfo) > inherited;
protected:
  // вспомогательна€ структура
  struct TBlockI : public TSLL_ListMemT< sizeof(TInfo) >::TBlock
  {
    TInfo   info;
  } _PACKED;

public:
//	TSLL_ListMemT();
//	~TSLL_ListMemT(); 

  // добавить пакет в конец списка 
  void append( void* data, int size, const TInfo &info );

  // ƒостать из списка первый пакет
	void* remoteFirst( int &size, TInfo &info );

  // первый элемент списка
  void* first( int &size, TInfo *&info );

  // элемент списка, следующий за указанным <item>
  void* next( void* item, int &size, TInfo *&info );
};
//-----------------------------------------------------------------------------

#include "ListMem_SingleLink_p.h"

#endif // ListMem_SingleLinkH
