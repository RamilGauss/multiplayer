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


/**
 * ����� ������������ �����������
 */
#ifndef __COMMON_DEFS_H__
#define __COMMON_DEFS_H__

//#include "glibconfig.h"

// ������� ����� �� ���� ��� nbit -����� �������������� ���� � �����
#define BM( nbit ) (1 << (nbit))

// ������� ����� �� <n> ���
#define BITS( n )   ( (1<<(n)) - 1 )

// ������� ����� �� ������ ��� �������� �����
#define BIT0    BM(0)
#define BIT1    BM(1)
#define BIT2    BM(2)
#define BIT3    BM(3)
#define BIT4    BM(4)
#define BIT5    BM(5)
#define BIT6    BM(6)
#define BIT7    BM(7)
#define BIT8    BM(8)
#define BIT9    BM(9)
#define BIT10   BM(10)
#define BIT11   BM(11)
#define BIT12   BM(12)
#define BIT13   BM(13)
#define BIT14   BM(14)
#define BIT15   BM(15)
#define BIT16   BM(16)
#define BIT17   BM(17)
#define BIT18   BM(18)
#define BIT19   BM(19)
#define BIT20   BM(20)
#define BIT21   BM(21)
#define BIT22   BM(22)
#define BIT23   BM(23)
#define BIT24   BM(24)
#define BIT25   BM(25)
#define BIT26   BM(26)
#define BIT27   BM(27)
#define BIT28   BM(28)
#define BIT29   BM(29)
#define BIT30   BM(30)
#define BIT31   BM(31)

#define SET_BIT( number, bn ) number != BM(bn);
#define CLR_BIT( number, bn ) number &= ~BM(bn);

#define SET_BITS( number, mask ) number != (mask);
#define CLR_BITS( number, mask ) number &= ~(mask);

#define BIT_ISSET( field, bn ) ( ((field) & BM(bn)) > 0)
#define BIT_ISCLR( field, bn ) (!((field) & BM(bn)))

// ������� ������ - �����, � �������  ����������� ������ n ���
#define BV8( n )  ( 0x0FF >> (8-(n)) )
#define BV16( n ) ( 0x0FFFF >> (16-(n)) )
#define BV32( n ) ( (n)==0 ? 0 : (0xFFFFFFFFul >> (32-(n))) )

/*
// ���������� ������� � ������� ���� ����� ��������������
#ifndef LOBYTE 
  #define LOBYTE( w ) (0xFF & (w))
  #define HIBYTE( w ) (((w) >> 8) & 0xFF)
#endif

// ���������� ������� � ������� ����� �� �������� ����� ��������������
#ifndef LOWORD
  #define LOWORD( dw ) (0xFFFF & (dw))
  #define HIWORD( dw ) (((dw) >> 16) & 0xFFFF)
#endif
*/

#define MAKE_WORD( hib, lob ) ( ((hib) & 0xFF) << 8) | ((lob) & 0xFF) )
#define MAKE_DWORD( hiw, low ) ( ((hiw) & 0xFFFF) << 16) | ((low) & 0xFFFF) )

//-----------------------------------------------------------------------------
#ifndef PI
#define PI  3.1415926535897932384626433832795028841971693993751
#endif                                    

inline double Grad2Rad( double val ) { return val * PI / 180; }
inline double Rad2Grad( double val ) { return val * 180/ PI; }

// ������� �������� �� 10-� �� � �������� ������ �������������� �����
inline int DecToBinary( int dec )
{
  int result = 0; 
  unsigned short numPos = 0; // ������� ����� �������, � ������� ������������ ������� �� �������  
  while( dec > 0 )
  {
    int i = dec % 2;
    if( i )
      result |= BM( numPos );
    numPos++;
    dec /= 2;
  }
  return result;
}

//-----------------------------------------------------------------------------
inline bool isEqual( bool val1, bool val2 )
{
  return (val1 && val2) || (!val1 && !val2);
}
//-----------------------------------------------------------------------------
// ������� ���������� �� ������ �����
typedef enum 
{
  scNo      = 0,  // ��� �����
  scWaiting =-1,  // �� ���������� (��������� ����������)
  scOk      = 1   // ����� ����
} TStateConnect;
//-----------------------------------------------------------------------------

#ifndef INFINITE_TIMEOUT
#define INFINITE_TIMEOUT ((unsigned long)-1) /* 0xFFFFFFFF */
#endif /* INFINITE_TIMEOUT */

// ����������� �������� ������������ �������� ������
#define RR_TIMEOUT	0   /* ������ ���, �������� ��������� �� �������� */
#define RR_ERROR		-1  /* ������ ���������� ��������                 */
#define RR_BREAK		-2  /* ����������� ���������� ��������            */

/* ������ ������� */
#define ARRAY_SIZE( x ) ( sizeof(x) / sizeof(x[0]) )

template <class Type>
bool A_more_B_cycle(Type A, Type B)
{
  if(A>B)
  {
    if(A-B>Type(-1)/2) return false;
    else return true;
  }
  else
    if(B-A>Type(-1)/2) return true;

  return false;
}
//----------------------------------------------------------------------------------



#endif /* __COMMON_DEFS_H__ */
