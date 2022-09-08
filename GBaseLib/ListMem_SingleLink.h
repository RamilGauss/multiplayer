#ifndef ListMem_SingleLinkH
#define ListMem_SingleLinkH

#include "TypeDef.h"

//--------------------------------------------------------------------------
template< size_t sizeInfo >
class TSLL_ListMemT
{
protected:
  // ��������� ����� �������� ������
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
  
  // ������� �� ������ ������ ����
  void* removeFirstBlock();

public:
	TSLL_ListMemT();
	~TSLL_ListMemT(); 

  // ���������� ������ ������� �������
  static void free( void* data );

  // ������������ ���� ������
  void clear();

  // ���-�� ������
  int count()     { return mCnt; }
    
  // ������ ����
  bool empty()    { return mCnt == 0; }

  // �������� ����� � ����� ������ 
  void append( void* data, int size );

  // ������� �� ������ ������ �����
	void* remoteFirst( int &size );

  // ������� � ������ ������ �����
  void returnFirst( void* data );

  // ������ ������� ������
  void* first( int &size );

  // ������� ������, ��������� �� ��������� <item>
  void* next( void* item, int &size );
};


typedef TSLL_ListMemT<0>    TSLL_ListMem;

//--------------------------------------------------------------------------

template< typename TInfo >
class TSLL_ListMemTI : public TSLL_ListMemT< sizeof(TInfo) >
{
  typedef TSLL_ListMemT< sizeof(TInfo) > inherited;
protected:
  // ��������������� ���������
  struct TBlockI : public TSLL_ListMemT< sizeof(TInfo) >::TBlock
  {
    TInfo   info;
  } _PACKED;

public:
//	TSLL_ListMemT();
//	~TSLL_ListMemT(); 

  // �������� ����� � ����� ������ 
  void append( void* data, int size, const TInfo &info );

  // ������� �� ������ ������ �����
	void* remoteFirst( int &size, TInfo &info );

  // ������ ������� ������
  void* first( int &size, TInfo *&info );

  // ������� ������, ��������� �� ��������� <item>
  void* next( void* item, int &size, TInfo *&info );
};
//-----------------------------------------------------------------------------

#include "ListMem_SingleLink_p.h"

#endif // ListMem_SingleLinkH
