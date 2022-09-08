/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


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
