#include "ToStr.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>

TN2A N2A;
//------------------------------------------------------------------------------
void TN2A::getSystemParam()
{
	struct lconv *plc = localeconv();
	thousands_sep = ( plc && plc->thousands_sep && *plc->thousands_sep ) ? *plc->thousands_sep : ' ';
	decimal_point = ( plc && plc->decimal_point && *plc->decimal_point ) ? *plc->decimal_point : ',';
	isInit = true;
}
//------------------------------------------------------------------------------
// Получить символ-разделитель групп десятичных разрядов числа на тетрады
// Символ возвращается в локальной кодировке
char TN2A::thousandsSep()
{
	init();
	return thousands_sep;
}
//------------------------------------------------------------------------------
// Установить символ-разделитель триад в разрядах числа
// Результат - предыдущее значение
char TN2A::thousandsSep( char value )
{
  init();
  char res = thousands_sep;
  thousands_sep = value;
  return res;
}
//------------------------------------------------------------------------------
// Получить символ-разделитель дробной части числа
char TN2A::floatSep()
{
	init();
	return decimal_point;
}
//------------------------------------------------------------------------------
// Установить символ-разделитель дробной части числа
// Результат - предыдущее значение
char TN2A::floatSep( char value )
{
  init();
  char res = decimal_point;
  decimal_point = value;
  return res;
}
//------------------------------------------------------------------------------
// Удаление триадных разделителей из строки
char* TN2A::clear( const char *src, char *dst )
{
	init();
	if( !dst ) dst = (char*)src;
  while( *src )
  {
    char c = *src++;
     if( c != thousands_sep )
      *dst++ = c;
  }
  *dst = '\0';
  return dst;
}
//-----------------------------------------------------------------------------
// Представление числа в символьной форме с прореживанием по 3 разряда
char* TN2A::conv( char* st, guint32 val )
{
  init();
  int cntToTetrade = 0;
  char* pStart = st;
  char* pst = pStart;

  while( val )
  {
    if( cntToTetrade == 3 )
    {
      *pst++ = thousands_sep;
      cntToTetrade = 1;
    }
    else cntToTetrade++;

    int newVal = val / 10;
    int ost = val - newVal*10;
    *pst++ = (char)('0' + ost);
    val = newVal;
  }
  size_t len = pst - pStart;
  if( len )
  {
    size_t half_len = len / 2;
    for( size_t i = 0; i < half_len; i++ )
    {
      char c = pStart[i];
      pStart[i] = pStart[len-1-i];
      pStart[len-1-i] = c;
    }
  }
  else *pst++ = '0';
  *pst = '\0';
  return st;
}
//------------------------------------------------------------------------------
char* TN2A::conv( char* st, int val )
{
  if( val < 0 )
  {
    st[0] = '-';
    conv( st+1, (guint32)(-val) );
  }
  else
    conv( st, (guint32)val );
  return st;
}
//------------------------------------------------------------------------------
char* TN2A::conv( char* st, guint64 val )
{
  init();
  int cntToTetrade = 0;
  char* pStart = st;
  char* pst = pStart;

  while( val )
  {
    if( cntToTetrade == 3 )
    {
      *pst++ = thousands_sep;
      cntToTetrade = 1;
    }
    else cntToTetrade++;

    guint64 newVal = val / 10;
    int ost = (int)(val - newVal*10);
    *pst++ = (char)('0' + ost);
    val = newVal;
  }
  size_t len = pst - pStart;
  if( len )
  {
    size_t half_len = len / 2;
    for( size_t i = 0; i < half_len; i++ )
    {
      char c = pStart[i];
      pStart[i] = pStart[len-1-i];
      pStart[len-1-i] = c;
    }
  }
  else *pst++ = '0';
  *pst = '\0';
  return st;
}
//------------------------------------------------------------------------------
char* TN2A::conv( char* st, gint64 val )
{
  if( val < 0 )
  {
    st[0] = '-';
    conv( st+1, (guint64)(-val) );
  }
  else conv( st, (guint64)val );
  return st;
}
//------------------------------------------------------------------------------
char* TN2A::conv( char* st, double val, int cntFloat )
{
  init();
  int len;
  if( val < 0 )
  {
    st[0] = '-';
    val = -val;
    len = 1;
  }
  else len = 0;

  gint64 mult = 1;
  for( int i = 0; i < cntFloat; i++ )
    mult *= 10;
  gint64 dval = (gint64)(val * mult + 0.5);

  len += (int)strlen( n2a( st+len, dval/mult ) );

  // Разбор дробной части
  if( cntFloat )
  {
    st[len++] = FloatSep_Get();
    gint64 dost = (gint64)(dval % mult);
    for( int i = 0; i < cntFloat; i++ )
    {
      mult /= 10;
      int v = (int)(dost / mult);
      dost -= v * mult;

      st[len++] = (char)('0' + v);
    }
    st[len] = '\0';
  }
  return st;
}
//------------------------------------------------------------------------------
