/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#include "BL_ConfigFile.h"
#include <stdio.h>
#include <string.h>
#include "BL_AppFile.h"
#include <glib.h>
#include "BL_Debug.h"
#include "BL_Conv.h"
#include <stdlib.h>

TAutoCreateVarT<TBL_ConfigFile> AppConfigFile;

//------------------------------------------------------------------------------
TBL_ConfigFile::TBL_ConfigFile()
{
  m_keyfile     = NULL;
  m_gerror      = NULL;
  m_szFileName  = NULL;
  
  defaultInit();
}
//------------------------------------------------------------------------------
TBL_ConfigFile::TBL_ConfigFile( bool autoSave )
{
  m_keyfile     = NULL;
  m_gerror      = NULL;
  m_szFileName  = NULL;
  autosave      = autoSave;
}
//------------------------------------------------------------------------------
// Название конфигурационного файла
const char* TBL_ConfigFile::fileName()
{
  return m_szFileName ? m_szFileName->str : NULL;
}
//------------------------------------------------------------------------------
void TBL_ConfigFile::defaultInit()
{
  autosave = true;
  
  char nameConfigFile[MAX_PATH];
#ifdef TD_WINDOWS
  char nameConfigFileUTF8[MAX_PATH*2];
  bl_L2UTF8( BL_AppFile_Ext(nameConfigFile,".ini"), nameConfigFileUTF8, sizeof(nameConfigFileUTF8) );
  Open( nameConfigFileUTF8 );
#else
  Open( BL_AppFile_Ext(nameConfigFile,".ini") );
#endif
}
//------------------------------------------------------------------------------
bool TBL_ConfigFile::checkOpen()
{
  if ( m_keyfile == NULL )
  {
    if( m_szFileName )
    {
      DoOpen( NULL );
      return m_keyfile != NULL;
    }
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
void TBL_ConfigFile::Open( const char *filename, const char* comment )
{
  Close();
  m_szFileName  = g_string_new( filename );
  if ( m_szFileName != NULL )
  {
    DoOpen( comment );
  }
}
//------------------------------------------------------------------------------
void TBL_ConfigFile::DoOpen( const char* comment )
{
  m_keyfile = g_key_file_new();
  if ( m_keyfile )
  {
    // преобразуем имя файла в кодировку GLib для имен файлов
    gchar* fn = g_filename_from_utf8( m_szFileName->str, -1, NULL, 0, &m_gerror  );//NULL
    if ( fn )
    {
      g_clear_error( &m_gerror );
      if ( ! g_key_file_load_from_file( m_keyfile, fn, G_KEY_FILE_KEEP_COMMENTS, &m_gerror) )
      {
        if( comment )
        {
          g_clear_error( &m_gerror );
          g_key_file_load_from_data(m_keyfile, (gchar*)comment, (gsize)strlen(comment),
                                         G_KEY_FILE_KEEP_COMMENTS, &m_gerror);
        }
      }
      else
      {
        if ( m_gerror != NULL)
        {
          g_key_file_free( m_keyfile );
          m_keyfile = NULL;
        }
      }
    }
    g_free( fn );
  }
}
//------------------------------------------------------------------------------
void TBL_ConfigFile::SaveAs( const char *NewFN)
{
  if ( m_szFileName != NULL )
    g_string_free( m_szFileName, true );

  m_szFileName = g_string_new( NewFN );

  FlushToFile();
}
//------------------------------------------------------------------------------
TBL_ConfigFile::~TBL_ConfigFile()
{
  Close();
}
//------------------------------------------------------------------------------
void TBL_ConfigFile::Close()
{
  if ( m_szFileName != NULL ) 
  {
    g_string_free( m_szFileName, true );
    m_szFileName = NULL;
  }

  if ( m_keyfile != NULL ) 
  {
    g_key_file_free( m_keyfile );
    m_keyfile = NULL;
  }
  g_clear_error( &m_gerror );
}

/********************************************************************************************
        GetValue - получает значение из конфигурационного файла, если группы или
                    параметра не найдено, устанавливает параметр по умолчанию
    in:
       group_name   - имя группы
       key          - имя параметра
       defaultValue - значение параметра по умолчанию, если параметр не был найден
    out:
      ASCIIZ строка со значением параметра
********************************************************************************************/
char *TBL_ConfigFile::GetValue (const char *group_name, const char *key, const char *defaultValue )
{
  if ( !checkOpen() )
    return defaultValue ? g_strdup ( defaultValue ) : NULL;

  g_clear_error( &m_gerror );
  gchar *res = g_key_file_get_value(m_keyfile, group_name, key, &m_gerror);

	if ( !res )
  {
    if ( autosave ) SetValue( group_name, key, defaultValue ? defaultValue : "" );
    return res = g_strdup( defaultValue );
  }
  return res;
}

/********************************************************************************************
        SetValue - устанавливает значение в конфигурационном файле
    in:
       group_name   - имя группы
       key          - имя параметра
       value        - значение параметра которое требуется установить
********************************************************************************************/
void TBL_ConfigFile::SetValue( const char *group_name, const char *key, const char *value )
{
  if ( checkOpen() )
  {
    g_clear_error( &m_gerror );
    g_key_file_set_value(m_keyfile, group_name, key, value);
    if ( autosave )
      FlushToFile();
  }
}
/********************************************************************************************
        GetBool - получает значение из конфигурационного файла, если группы или
                    параметра не найдено, устанавливает параметр по умолчанию
    in:
       group_name   - имя группы
       key          - имя параметра
       defaultValue - значение параметра по умолчанию, если параметр не был найден
    out:
    //  ???
********************************************************************************************/
bool     TBL_ConfigFile::GetBool (const char *group_name, const char *key, bool defaultValue )
{
  if ( !checkOpen() )
    return defaultValue;

  g_clear_error( &m_gerror );
  bool res = g_key_file_get_boolean (m_keyfile, group_name, key, &m_gerror) ? true : false;

  if ( !m_gerror )
  {
    return res;
  }
  else
  {
    if ( autosave ) SetBool( group_name, key, defaultValue );
    return defaultValue;
  }
 }
/********************************************************************************************
        SetBool - устанавливает значение в конфигурационном файле
    in:
       group_name   - имя группы
       key          - имя параметра
       value        - значение параметра которое требуется установить
********************************************************************************************/
void       TBL_ConfigFile::SetBool( const char *group_name, const char *key, bool value )
{
  if ( checkOpen() )
  {
    g_clear_error( &m_gerror );
    g_key_file_set_boolean(m_keyfile, group_name, key, value);
    if ( autosave )
      FlushToFile();
  }
 }
/********************************************************************************************
        GetInteger - получает значение типа int из конфигурационного файла, если группы или
                     параметра не найдено, устанавливает параметр по умолчанию
    in:
       group_name   - имя группы
       key          - имя параметра
       defaultValue - значение параметра по умолчанию, если параметр не был найден
    out:
      значение, считанное из конфигурационного файла
********************************************************************************************/
gint  TBL_ConfigFile::GetInteger (const char *group_name, const char *key, gint defaultValue)
{
  if ( !checkOpen() )
    return defaultValue;

  g_clear_error( &m_gerror );
  gint res = g_key_file_get_integer(m_keyfile, group_name, key, &m_gerror);

  if ( m_gerror != NULL )
  {// параметра ранее не существовало, создадим его
    if ( autosave ) SetInteger(group_name, key, defaultValue );
    return defaultValue;
  }else{
    return res;
  }
}

/********************************************************************************************
        SetInteger  - устанавливает значение типа int в конфигурационном файле
    in:
       group_name   - имя группы
       key          - имя параметра
       value        - значение параметра которое требуется установить
********************************************************************************************/
void TBL_ConfigFile::SetInteger (const char *group_name, const char *key, gint value )
{
  if ( checkOpen() )
  {
    g_clear_error( &m_gerror );
    g_key_file_set_integer(m_keyfile, group_name, key, value );
    if ( autosave ) FlushToFile();
  }
}

/********************************************************************************************
        GetDouble - получает значение типа double из конфигурационного файла, если группы или
                     параметра не найдено, устанавливает параметр по умолчанию
    in:
       group_name   - имя группы
       key          - имя параметра
       defaultValue - значение параметра по умолчанию, если параметр не был найден
    out:
      значение, считанное из конфигурационного файла
********************************************************************************************/
double TBL_ConfigFile::GetDouble(const char *group_name, const char *key, double defaultValue )
{
  if ( !checkOpen() )
    return defaultValue;

  g_clear_error( &m_gerror );
#if GLIB_CHECK_VERSION(2,12,0)
  gdouble res = g_key_file_get_double(m_keyfile, group_name, key, &m_gerror);
  if ( m_gerror != NULL )
  {// параметра ранее не существовало, создадим его
    if ( autosave ) SetDouble(group_name, key, defaultValue );
    return defaultValue;
  }
  else return res;
#else
	gdouble res;// #### sick!
  //gfloat res;
  gchar *resSt = g_key_file_get_value(m_keyfile, group_name, key, &m_gerror);

  res = atof(resSt);

  if( !resSt || m_gerror || !sscanf( resSt, "%f", &res ) )
  {// параметра ранее не существовало, создадим его
    if ( autosave ) SetDouble(group_name, key, defaultValue );
    res = defaultValue;
  }
  if( resSt ) g_free( resSt );
  return res;
#endif
}

/********************************************************************************************
        SetInteger  - устанавливает значение типа int в конфигурационном файле
    in:
       group_name   - имя группы
       key          - имя параметра
       value        - значение параметра которое требуется установить
********************************************************************************************/
void TBL_ConfigFile::SetDouble(const char *group_name, const char *key, double value )
{
  if ( checkOpen() )
  {
    g_clear_error( &m_gerror );
#if GLIB_CHECK_VERSION(2,12,0)
    g_key_file_set_double(m_keyfile, group_name, key, value );
#else
    char st[50];
    sprintf( st, "%f", value );
    g_key_file_set_value(m_keyfile, group_name, key, st );
#endif
    if ( autosave ) FlushToFile();
  }
}


//********************************************************************************************
// Возвращает массив строк
char**  TBL_ConfigFile::GetListFromKey( const char *group_name, const char *key, gsize &count)
{
  if ( !checkOpen() )
    return NULL;

  if (&m_gerror)
    g_clear_error( &m_gerror );
  return g_key_file_get_string_list( m_keyfile, group_name, key, &count, &m_gerror );
}

//********************************************************************************************
// Записывает список строковых значений
void  TBL_ConfigFile::SetListToKey( const char *group_name,    const char *key,
                   					     const char * const list[], gsize length )
{
  if ( !checkOpen() ) return;

  g_key_file_set_string_list( m_keyfile, group_name, key, list, length );
  if ( autosave )
    FlushToFile();
}
//********************************************************************************************
// Имеет ли ключевой файл группу group_name
bool TBL_ConfigFile::HasGroup( const gchar *group_name )
{
  if ( !checkOpen() ) return false;

  return !!g_key_file_has_group( m_keyfile, group_name );
}
//********************************************************************************************
// Имеет ли ключевой файл ключ key в группе group_name
bool TBL_ConfigFile::HasKey( const gchar *group_name, const gchar *key )
{
  if ( !checkOpen() ) return false;
  g_clear_error( &m_gerror );
  return !!g_key_file_has_key ( m_keyfile, group_name, key, &m_gerror );
}
//********************************************************************************************
// Названия всех групп в файле m_keyfile
char**  TBL_ConfigFile::GetAllGroups( gsize &count )
{
  if ( m_keyfile == NULL ) return NULL;
  if (&m_gerror)
    g_clear_error( &m_gerror );
  return g_key_file_get_groups( m_keyfile, &count );
}
//********************************************************************************************
// Возвращает все ключи для группы
char**  TBL_ConfigFile::GetKeysFromGroup ( const char *group_name, gsize &length)
{
  if ( !checkOpen() )
    return NULL;

  if (&m_gerror)
    g_clear_error( &m_gerror );
  return g_key_file_get_keys( m_keyfile, group_name, &length, &m_gerror );
}
//********************************************************************************************
// RemoveGroup - удаляет группу group_name
void  TBL_ConfigFile::RemoveGroup( const char *group_name )
{
  if ( !checkOpen() ) return;

  g_clear_error( &m_gerror );
  g_key_file_remove_group( m_keyfile, group_name, &m_gerror );
  if ( autosave )
    FlushToFile();
}

//********************************************************************************************
// Удаляет ключ key в группе group_name
void  TBL_ConfigFile::RemoveKey( const char *group_name, const char *key)
{
  if ( !checkOpen() ) return;

  g_clear_error( &m_gerror );
  g_key_file_remove_key( m_keyfile, group_name, key, &m_gerror );
  if ( autosave )
    FlushToFile();
}

//********************************************************************************************
// Удаляет содержимое файла
//void  TBL_ConfigFile::RemoveContent( const char *filename )
//{
//  Close();                    // закрываем ключевой файл
//  m_keyfile     = NULL;
//  m_gerror      = NULL;
//  m_szFileName  = NULL;
//  if ( g_file_test( filename, G_FILE_TEST_EXISTS ) ) // если файл существует
//    unlink( filename );          // удаляем файл
//}



//********************************************************************************************
// запись данных в файл
void TBL_ConfigFile::FlushToFile()
{
  if ( checkOpen() )
  {
    gsize length;
    g_clear_error( &m_gerror );
    char *data = g_key_file_to_data( m_keyfile, &length, &m_gerror );
    // если у нас есть данные для сохранения, то сохраним их
    if ( data != NULL )
    {
      char* fn = g_locale_from_utf8( m_szFileName->str, -1, NULL, 0, NULL );
      if( fn )
      {
        FILE *out = fopen( fn, "wt" );
        if ( out != NULL )
        {
          fwrite(data, 1, length, out);
          fclose(out);
        }
        g_free( fn );
      }
      g_free( data );
    }
  }
}
//********************************************************************************************
// Запросить комментарий для группы или ключа
char *TBL_ConfigFile::GetComment( const char *group_name, const char *key )
{
  if ( !checkOpen() )
    return NULL;
  
  g_clear_error( &m_gerror );
  char *c = g_key_file_get_comment( m_keyfile, group_name, key, &m_gerror );
  return c;
}
//********************************************************************************************
// Задать комментарий для группы или ключа
void TBL_ConfigFile::SetComment( const char *group_name, const char *key, const char *comment )
{
  if ( !checkOpen() )
    return;
  
  g_clear_error( &m_gerror );
  g_key_file_set_comment( m_keyfile, group_name, key, comment, &m_gerror );
  if ( autosave )
    FlushToFile();
}
//********************************************************************************************
// Удаляет комментарий выше key из group_name. 
void TBL_ConfigFile::RemoveComment( const char *group_name, const char *key )
{
  if ( !checkOpen() )
    return;

  g_clear_error( &m_gerror );
  g_key_file_remove_comment( m_keyfile, group_name, key, &m_gerror );
  if ( autosave )
    FlushToFile();
}
//********************************************************************************************
