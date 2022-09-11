/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_ConfigFileH
#define BL_ConfigFileH

#include "TypeDef.h"
#include <glib/gstring.h>
#include <glib/gkeyfile.h>
#include "AutoCreateVar.h"

class GBASELIB_EI TBL_ConfigFile
{
public:
  // c��������� ������� ��-���������
  TBL_ConfigFile();

  // autoSave - ���������� ������  � ���������������� ����� �� ����� ����� ������
  //            �������� ��������� ���������, � ����� �������������� ����������
  //            ������������� ���������� ���������� �� ��������� ��� ���������
  //            ������ ����������
  TBL_ConfigFile( bool autoSave );
  ~TBL_ConfigFile();

  // ��������� ������� ��-���������
  // (�������������� � ���� <����� ����������>\<�������� ����������>.ini)
  void defaultInit();

  // ���������� ����� ��������������
  // ���������: ���������� ��������� ���������
  bool setAutoSaveMode( bool autoSave )     { bool res = autosave; autosave = autoSave; return res; }

  // �������� ����������������� �����
  const char* fileName();

  //********************************************************************************************
  // ��������� ����������� ��� ������ ��� �����
  // ����.: �������� ����� ������������� ���������� g_free( _result_ )
  char *GetComment( const char *group_name, const char *key );

  // ��������� ����������� ���� key �� group_name. ���� key ��� NULL, �� comment ����� ������� 
  // ������ group_name. ���� � key � group_name �������� NULL, �� comment ����� ������� ������ 
  // ������ ������ � �����.
  void SetComment( const char *group_name, const char *key, const char *comment );

  // ������� ����������� ���� key �� group_name. 
  void RemoveComment( const char *group_name, const char *key );
  //********************************************************************************************

  // ���������� ��������� ���������� ������� � ������
  bool IsOpen(void) { return m_keyfile != NULL; }

  // ��������� ��� ������� ���� <filename>
  // filename - ��� ����� � UTF-8
  // comment  - ������������ ����������� ����� (������ � ������ �������� ������ �����)
  void Open( const char *filename, const char* comment = NULL);

  // �������
  void Close();

  // ��������� � ����
  void Save()   { FlushToFile(); }

  // ��������� � ���� c ����� ���������
  void SaveAs( const gchar *NewFN );

  //********************************************************************************************
  // �������� �������� �� ����������������� �����.
  // ASCIIZ ������ �� ��������� ���������.
  // ����.: �������� ����� ������������� ���������� g_free( _result_ )
  char *GetValue (const char *group_name, const char *key, const char *defaultValue = NULL);
  void SetValue( const char *group_name, const char *key, const char *value );

  bool GetBool (const char *group_name, const char *key, bool defaultValue = false);
  void SetBool( const char *group_name, const char *key, bool value );

  int GetInteger (const char *group_name, const char *key, int defaultValue = 0);
  void SetInteger (const char *group_name, const char *key, int value );

  double GetDouble(const char *group_name, const char *key, double defaultValue = 0);
  void SetDouble(const char *group_name, const char *key, double value );

  //********************************************************************************************
  // ���������� ������ �����
  // group_name - ��� ������
  // key        - ����
  // count      - ���������� ������������ �����
  // ����.: �������� ����� ������������� ���������� g_strfreev( _result_ )
  gchar**  GetListFromKey( const gchar *group_name, const gchar *key, gsize &count );

  // ���������� ������ ��������� ��������
  //  group_name    -   ��� ������
  //  key           -   ����
  //  list          -   ������ �����
  //  count         -   ���������� �����, ������� ���� ���������
  // ����.: 1) ������ � ��������� UTF-8. 
  void  SetListToKey( const gchar *group_name, const gchar *key,
        					    const gchar * const list[], gsize count );

  //********************************************************************************************
  // ����� �� �������� ���� ������ group_name
  bool HasGroup( const gchar *group_name );
  
  // ����� �� �������� ���� ���� key � ������ group_name
  bool HasKey( const gchar *group_name, const gchar *key );

  //********************************************************************************************
  // �������� ���� �����
  // count - ���������� �����
  // ����.: �������� ����� ������������� ���������� g_strfreev( _result_ )
  gchar** GetAllGroups( gsize &count );

  // ���������� ��� ����� ��� ������
  // count - ���������� ������������ ������
  // ����.: �������� ����� ������������� ���������� g_strfreev( _result_ )
  gchar**  GetKeysFromGroup ( const gchar *group_name, gsize &count );

  //********************************************************************************************
  // RemoveGroup - ������� ������ group_name
  // group_name    : 	 ��� ������
  void RemoveGroup( const gchar *group_name );

  // ������� ���� key � ������ group_name
  // group_name    : 	 ��� ������
  // key           : 	 ��� ����� ��� ��������
  void RemoveKey( const gchar *group_name, const gchar *key);


  //********************************************************************************************
  
  // ��������� ������ ��� ���������� �������� ������ � ��������
  GError *GetLastError() { return m_gerror; }
private:
  void      FlushToFile();
  bool      checkOpen();
  void      DoOpen( const char* comment );
protected:

  bool     autosave;
  GKeyFile *m_keyfile;
  GError   *m_gerror;
  GString  *m_szFileName;
};

//---------------------------------------------------------------------------

// ������ ���������������� ���� ����������
extern GBASELIB_EI TAutoCreateVarT<TBL_ConfigFile> AppConfigFile;

//---------------------------------------------------------------------------
#endif

