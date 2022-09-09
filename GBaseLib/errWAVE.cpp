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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


// Windows platform: wave API

#include "TypeDef.h"
#include "ErrorReg.h"
#include <windows.h>

static guint8 egWAVEIN;
static guint8 egWAVEOUT;
static guint8 egWAVEMIX;
//------------------------------------------------------------------------------
static int GetStrIN( guint32 code, char* buf, int len )
{
  if( ::waveInGetErrorText( code, buf, len ) != MMSYSERR_NOERROR )
  {
    lstrcpyn( buf, "�������� ������ �� ��������", len );
  }
  return lstrlen( buf );
}
//------------------------------------------------------------------------------
static int GetStrOUT( guint32 code, char* buf, int len )
{
  if( ::waveOutGetErrorText( code, buf, len ) != MMSYSERR_NOERROR )
  {
    lstrcpyn( buf, "�������� ������ �� ��������", len );
  }
  return lstrlen( buf );
}
//------------------------------------------------------------------------------
static int _waveMixerGetErrorText( int code, char* buf, int& len )
{
	switch(code)
	{
		case MMSYSERR_NOERROR      :/* no error */
			lstrcpyn( buf, "��� ������", len );break;
		case MMSYSERR_ERROR        :/* unspecified error */
			lstrcpyn( buf, "����������� ������", len );break;
		case MMSYSERR_BADDEVICEID  :/* device ID out of range */
			lstrcpyn( buf, "������������ �������� ID ���������� ", len );break;
		case MMSYSERR_NOTENABLED   :/* driver failed enable */
			lstrcpyn( buf, "������� �� ��������", len );break;
		case MMSYSERR_ALLOCATED    :/* device already allocated */
			lstrcpyn( buf, "���������� ��� ��������", len );break;
		case MMSYSERR_INVALHANDLE  :/* device handle is invalid */
			lstrcpyn( buf, "���������� ���������� ��������������", len );break;
		case MMSYSERR_NODRIVER     :/* no device driver present */
			lstrcpyn( buf, "� ���������� ��� ��������", len );break;
		case MMSYSERR_NOMEM        :/* memory allocation error */
			lstrcpyn( buf, "������ ��������� ������", len );break;
		case MMSYSERR_NOTSUPPORTED :/* function isn't supported */
			lstrcpyn( buf, "������� �� ��������������", len );break;
		case MMSYSERR_BADERRNUM    :/* error value out of range */
			lstrcpyn( buf, "������������ ��������", len );break;
		case MMSYSERR_INVALFLAG    :/* invalid flag passed */
			lstrcpyn( buf, "������������ �������� �����", len );break;
		case MMSYSERR_INVALPARAM   :/* invalid parameter passed */
			lstrcpyn( buf, "������������ �������� ���������", len );break;
		case MMSYSERR_HANDLEBUSY   :/* handle being used */
			lstrcpyn( buf, "���������� ��� ������������ � ������ ������", len );break;
		case MMSYSERR_INVALIDALIAS :/* specified alias not found */
			lstrcpyn( buf, "��������� �������� �� �������", len );break;
		case MMSYSERR_BADDB        :/* bad registry database */
			lstrcpyn( buf, "������: ������������ ������", len );break;
		case MMSYSERR_KEYNOTFOUND  :/* registry key not found */
			lstrcpyn( buf, "������: ���� �� ������", len );break;
		case MMSYSERR_READERROR    :/* registry read error */
			lstrcpyn( buf, "������: ������ ������", len );break;
		case MMSYSERR_WRITEERROR   :/* registry write error */
			lstrcpyn( buf, "������: ������ ������", len );break;
		case MMSYSERR_DELETEERROR  :/* registry delete error */
			lstrcpyn( buf, "������: ������ ��������", len );break;
		case MMSYSERR_VALNOTFOUND  :/* registry value not found */
			lstrcpyn( buf, "������: �������� �� ���� �������", len );break;
		case MMSYSERR_NODRIVERCB   :/* driver does not call DriverCallback */
			lstrcpyn( buf, "������� �� ������ DriverCallback", len );break;
		case MMSYSERR_MOREDATA     :/* more data to be returned */
			lstrcpyn( buf, "������� ������� ����� ������ ������������", len );break;
		default:;
	}
  return 0;
}
//------------------------------------------------------------------------------
static int GetStrMIX( guint32 code, char* buf, int len )
{
	if( _waveMixerGetErrorText( code, buf, len ) != MMSYSERR_NOERROR )
	{
		lstrcpyn( buf, "�������� ������ �� ��������", len );
	}
	return lstrlen( buf );
}
//------------------------------------------------------------------------------
static TDefErrorGroup degIN = {
  GetStrIN,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
static TDefErrorGroup degOUT = {
  GetStrOUT,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
static TDefErrorGroup degMIX = {
	GetStrMIX,
	NULL,
	NULL
};
//------------------------------------------------------------------------------
void errWAVEIN_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egWAVEIN, code, file, line, source );
}
//------------------------------------------------------------------------------
void errWAVEOUT_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egWAVEOUT, code, file, line, source );
}
//------------------------------------------------------------------------------
void errWAVEMIX_Set( guint32 code, const char* file, int line, void* source )
{
	err_SetError( egWAVEMIX, code, file, line, source );
}
//------------------------------------------------------------------------------
bool errWAVE_Init(void)
{
  if( !egWAVEIN )
    egWAVEIN = err_RegisterGroup( &degIN );
  if( !egWAVEOUT )
    egWAVEOUT = err_RegisterGroup( &degOUT );
	if( !egWAVEMIX )
		egWAVEMIX = err_RegisterGroup( &degMIX );

  return egWAVEIN && egWAVEOUT && egWAVEMIX;
}
//------------------------------------------------------------------------------
