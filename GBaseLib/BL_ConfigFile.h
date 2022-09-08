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

