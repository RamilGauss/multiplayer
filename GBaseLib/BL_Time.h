/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef BL_TimeH
#define BL_TimeH

#include "TypeDef.h"
#include <time.h>

// ���������� �����������, ��������� � 1 ������ 1970 �.
typedef gint64 mtime_t;

//---------------------------------------------------------------------
// �������� ������� ��������� ����� 
GBASELIB_EI mtime_t mt_sysTime();

// �������� ������� ����������� ����� (� ����������)
GBASELIB_EI mtime_t mt_time();

// ������� ����������� �����
inline time_t bl_time()  { return (time_t)( mt_time()/1000 ); }
//---------------------------------------------------------------------
// ������������� time_t � mtime_t 
inline mtime_t mt_fromTime_t( time_t time ) { return time * 1000;}

// ������������� mtime_t � time_t
inline time_t  mt_toTime_t( mtime_t time )  { return (time_t)( time / 1000); }

// ������������� mtime_t � tm
GBASELIB_EI tm mt_toTm( mtime_t curTime );
//---------------------------------------------------------------------
// ��������� ���������� ������������ ������� ��� ������ ��������
// val - ����� �������������� �� ��������� ���
GBASELIB_EI void mt_correctByPrecision( mtime_t val );

// ��������������� ����������� �����
// delta - �������� ���������, ���
GBASELIB_EI void bl_time_correct( int delta );

//---------------------------------------------------------------------
// �������������� �� ���������� ������� �� ����� �� �������� � �������
time_t GBASELIB_EI time_LocalToGMT( time_t t );
time_t GBASELIB_EI time_GMTToLocal( time_t t );

// ������ �� ��������� ��������� ����� � ������ �����
// ���� ����� �� ������, �� ��������������� ������� �����
bool GBASELIB_EI IsSummerTime( time_t *curTime = 0 );

//------------------------------------------------------------------------------
// ��������������� ��������� ����� �� ������������
void GBASELIB_EI bl_time_to_system();
//------------------------------------------------------------------------------

#endif
