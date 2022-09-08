#ifndef BL_ConfigFileH
#define BL_ConfigFileH

#include "TypeDef.h"
#include <glib/gstring.h>
#include <glib/gkeyfile.h>
#include "AutoCreateVar.h"

class GBASELIB_EI TBL_ConfigFile
{
public:
  // cНастройка объекта по-умолчанию
  TBL_ConfigFile();

  // autoSave - сохранение данных  в конфигурационном файле на диске после каждой
  //            операции установки параметра, а также автоматическое сохранение
  //            отсутствующих параметров значениями по умолчанию при операциях
  //            чтения параметров
  TBL_ConfigFile( bool autoSave );
  ~TBL_ConfigFile();

  // Настройка объекта по-умолчанию
  // (автосохранение в файл <папка приложения>\<название приложения>.ini)
  void defaultInit();

  // Установить режим автосохранения
  // Результат: предыдущее состояние параметра
  bool setAutoSaveMode( bool autoSave )     { bool res = autosave; autosave = autoSave; return res; }

  // Название конфигурационного файла
  const char* fileName();

  //********************************************************************************************
  // Запросить комментарий для группы или ключа
  // Прим.: значение после использования освободить g_free( _result_ )
  char *GetComment( const char *group_name, const char *key );

  // Размещает комментарий выше key из group_name. Если key это NULL, то comment будет записан 
  // вверху group_name. Если и key и group_name являются NULL, то comment будет записан сверху 
  // первой группы в файле.
  void SetComment( const char *group_name, const char *key, const char *comment );

  // Удаляет комментарий выше key из group_name. 
  void RemoveComment( const char *group_name, const char *key );
  //********************************************************************************************

  // Возвращает состояние готовности объекта к работе
  bool IsOpen(void) { return m_keyfile != NULL; }

  // загрузить или создать файл <filename>
  // filename - имя файла в UTF-8
  // comment  - заголовочный комментарий файла (только в случае создания нового файла)
  void Open( const char *filename, const char* comment = NULL);

  // Закрыть
  void Close();

  // Сохранить в файл
  void Save()   { FlushToFile(); }

  // Сохранить в файл c новым названием
  void SaveAs( const gchar *NewFN );

  //********************************************************************************************
  // Получить значение из конфигурационного файла.
  // ASCIIZ строка со значением параметра.
  // Прим.: значение после использования освободить g_free( _result_ )
  char *GetValue (const char *group_name, const char *key, const char *defaultValue = NULL);
  void SetValue( const char *group_name, const char *key, const char *value );

  bool GetBool (const char *group_name, const char *key, bool defaultValue = false);
  void SetBool( const char *group_name, const char *key, bool value );

  int GetInteger (const char *group_name, const char *key, int defaultValue = 0);
  void SetInteger (const char *group_name, const char *key, int value );

  double GetDouble(const char *group_name, const char *key, double defaultValue = 0);
  void SetDouble(const char *group_name, const char *key, double value );

  //********************************************************************************************
  // Возвращает массив строк
  // group_name - имя группы
  // key        - ключ
  // count      - количество возвращённых строк
  // Прим.: значение после использования освободить g_strfreev( _result_ )
  gchar**  GetListFromKey( const gchar *group_name, const gchar *key, gsize &count );

  // Записывает список строковых значений
  //  group_name    -   имя группы
  //  key           -   ключ
  //  list          -   массив строк
  //  count         -   количество строк, которые надо сохранить
  // Прим.: 1) строки в кодировке UTF-8. 
  void  SetListToKey( const gchar *group_name, const gchar *key,
        					    const gchar * const list[], gsize count );

  //********************************************************************************************
  // Имеет ли ключевой файл группу group_name
  bool HasGroup( const gchar *group_name );
  
  // Имеет ли ключевой файл ключ key в группе group_name
  bool HasKey( const gchar *group_name, const gchar *key );

  //********************************************************************************************
  // Названия всех групп
  // count - количество групп
  // Прим.: значение после использования освободить g_strfreev( _result_ )
  gchar** GetAllGroups( gsize &count );

  // Возвращает все ключи для группы
  // count - количество возвращённых ключей
  // Прим.: значение после использования освободить g_strfreev( _result_ )
  gchar**  GetKeysFromGroup ( const gchar *group_name, gsize &count );

  //********************************************************************************************
  // RemoveGroup - удаляет группу group_name
  // group_name    : 	 имя группы
  void RemoveGroup( const gchar *group_name );

  // Удаляет ключ key в группе group_name
  // group_name    : 	 имя группы
  // key           : 	 имя ключа для удаления
  void RemoveKey( const gchar *group_name, const gchar *key);


  //********************************************************************************************
  
  // последняя ошибка при выполнении операции работы с конфигом
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

// Единый конфигурационный файл приложения
extern GBASELIB_EI TAutoCreateVarT<TBL_ConfigFile> AppConfigFile;

//---------------------------------------------------------------------------
#endif

