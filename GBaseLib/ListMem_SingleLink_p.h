#ifndef ListMem_SingleLink_pH
#define ListMem_SingleLink_pH

#include <string.h>
#include <glib.h>

#if (GLIB_MAJOR_VERSION > 2) || (GLIB_MAJOR_VERSION == 2 && GLIB_MINOR_VERSION > 8)
  #define glib_gslice_enable
  #include <glib/gslice.h>
#endif
//==============================================================================
//==============================================================================
//==============================================================================
template< size_t sizeInfo >
TSLL_ListMemT<sizeInfo>::TSLL_ListMemT()
{
	pFirst = pLast = 0;
	mCnt   = 0;
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
TSLL_ListMemT<sizeInfo>::~TSLL_ListMemT(void)
{
  clear();
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline /*static*/ size_t TSLL_ListMemT<sizeInfo>::sizeHeader() 
{ 
  return sizeof(TBlock) + sizeInfo; 
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline /*static*/ char* TSLL_ListMemT<sizeInfo>::blockToData( void* block ) 
{ 
  return ((char*)block) + sizeHeader(); 
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline /*static*/ void* TSLL_ListMemT<sizeInfo>::dataToBlock( void* data )
{ 
  return ((char*)data) - sizeHeader(); 
}
//------------------------------------------------------------------------------
// Освободить память занятую пакетом
template< size_t sizeInfo >
inline /*static*/ void TSLL_ListMemT<sizeInfo>::free( void* data )
{
  TBlock* pBlock = (TBlock*)dataToBlock( data );
  #ifdef  glib_gslice_enable
    g_slice_free1( sizeHeader() + pBlock->size, pBlock );
  #else
		::free( pBlock );
  #endif
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
void TSLL_ListMemT<sizeInfo>::append( void* data, int size )
{
  #ifdef  glib_gslice_enable
    char* cBlock = (char*)g_slice_alloc( (gsize)sizeHeader() + size );
  #else
    char* cBlock = (char*)malloc( sizeHeader() + size );
  #endif
  TBlock* pBlock = (TBlock*)cBlock;
	pBlock->pNext = NULL;
  pBlock->size = size;
  memcpy( cBlock + sizeHeader(), data, size );
  // Включаем в список
  if( pFirst == NULL )
		pFirst = pBlock;
	else
	  pLast->pNext = pBlock;
  mCnt++;
  pLast = pBlock;
}
//------------------------------------------------------------------------------
// Достать из списка первый блок
template< size_t sizeInfo >
inline void* TSLL_ListMemT<sizeInfo>::removeFirstBlock()
{
	TBlock* pBlock;
	if( pFirst )
	{
	  mCnt--;
	  pBlock = pFirst;
	  pFirst = pBlock->pNext;
    return pBlock;
  }
  return 0;
}
//------------------------------------------------------------------------------
// Достать из списка первый пакет
template< size_t sizeInfo >
inline void* TSLL_ListMemT<sizeInfo>::remoteFirst( int &size )
{
  TBlock* pBlock = (TBlock*)removeFirstBlock();
	if( pBlock )
	{
    size = pBlock->size;
    return blockToData( pBlock );
  }

  return NULL;
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline void TSLL_ListMemT<sizeInfo>::returnFirst( void* data )
{
  TBlock* pBlock = (TBlock*)dataToBlock(data);
  pBlock->pNext = pFirst;
  pFirst = pBlock;
  if( !pLast ) pLast = pBlock;
  mCnt++;
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline void* TSLL_ListMemT<sizeInfo>::first( int &size )
{
  if( pFirst )
  {
    size = pFirst->size;
    return blockToData( pFirst );
  }
  return NULL;
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
inline void* TSLL_ListMemT<sizeInfo>::next( void* item, int &size )
{
  TBlock* pBlock = (TBlock*)dataToBlock(item);
  pBlock = pBlock->pNext;
  if( pBlock )
  {
    size = pBlock->size;
    return blockToData( pBlock );
  }
  return NULL;
}
//------------------------------------------------------------------------------
template< size_t sizeInfo >
void TSLL_ListMemT<sizeInfo>::clear()
{
	TBlock* pBlock = pFirst;
	while( pBlock )
	{
		TBlock* pNext = pBlock->pNext;
    #ifdef  glib_gslice_enable
	  	g_slice_free1( (gsize)sizeHeader() + pBlock->size, pBlock );
    #else
			::free( pBlock );
    #endif
		pBlock = pNext;
	}
	pFirst = pLast = 0;
  mCnt = 0;
}
//------------------------------------------------------------------------------
//==============================================================================
//==============================================================================
//==============================================================================
template<typename TInfo>
inline void TSLL_ListMemTI<TInfo>::append( void* data, int size, const TInfo &info )
{
  inherited::append( data, size );
  ((TBlockI*)inherited::pLast)->info = info;
}
//------------------------------------------------------------------------------
template< typename TInfo>
inline void* TSLL_ListMemTI<TInfo>::remoteFirst( int &size, TInfo &info )
{
  TBlockI* pBlock = (TBlockI*)inherited::removeFirstBlock();
	if( pBlock )
	{
    size = pBlock->size;
    info = pBlock->info;
    return blockToData( pBlock );
  }
  return 0;
}
//------------------------------------------------------------------------------
template< typename TInfo>
inline void* TSLL_ListMemTI<TInfo>::first( int &size, TInfo *&info )
{
  if( TSLL_ListMemT<sizeof(TInfo)>::pFirst )
  {
    TBlockI* pBlock = (TBlockI*)TSLL_ListMemT<sizeof(TInfo)>::pFirst;
    size = pBlock->size;
    info = &pBlock->info;
    return TSLL_ListMemT< sizeof(TInfo) >::blockToData( pBlock );
  }
  return NULL;
}
//------------------------------------------------------------------------------
template< typename TInfo>
inline void* TSLL_ListMemTI<TInfo>::next( void* item, int &size, TInfo *&info )
{
  TBlockI* pBlock = (TBlockI*)TSLL_ListMemT<sizeof(TInfo)>::dataToBlock(item);
  pBlock = (TBlockI*)pBlock->pNext;
  if( pBlock )
  {
    size = pBlock->size;
    info = &pBlock->info;
    return TSLL_ListMemT<sizeof(TInfo)>::blockToData( pBlock );
  }
  return NULL;
}
//------------------------------------------------------------------------------


#endif //ListMem_SingleLink_pH
