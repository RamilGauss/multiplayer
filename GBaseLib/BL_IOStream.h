/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#ifndef BL_IOStreamH
#define BL_IOStreamH

#include "TypeDef.h"
#include <stdarg.h>


// Интерфейс управление потоком ввода/вывода
class GBASELIB_EI TIOStream
{
public:
  TIOStream(void);
  virtual ~TIOStream(void) { }


  //.......................................................
public: // Управление источником отказов
  //.......................................................
  
  // Назначенный источник отказов
  virtual void* errSource() const  { return NULL; }

  // Задать значение источника отказов
  virtual void setErrSource( void* ) { }

  //.......................................................
public: // Чтение/запись данных
  //.......................................................

  // Прочитать данные из потока
  // buf - приемный буфер
  // size - размер буфера
  // Результат: объем прочитанных данных или -1 в случае отказа
  virtual int read( void* buf, guint size ) = 0;

  // Записать данные в поток
  // buf - исходный буфер
  // size - объем данных
  // Результат: объем данных записанных данных или -1 в случае отказа
  virtual int write( const void* buf, guint size ) = 0;

  // Прочитать данные из потока
  // buf - приемный буфер
  // size - размер буфера
  // Результат: данные прочитаны в полном объеме
  bool get( void* buf, guint size );

  // Записать данные в поток
  // buf - исходный буфер
  // size - объем данных
  // Результат: данные записаны в полном объеме
  bool put( const void* buf, guint size );

  //.......................................................
public: // Чтение/запись данных
  //.......................................................
  bool put( signed char val )     { return put( &val, sizeof(val) ); }
  bool put( unsigned char val )   { return put( &val, sizeof(val) ); }
  bool put( signed short val )    { return put( &val, sizeof(val) ); }
  bool put( unsigned short val )  { return put( &val, sizeof(val) ); }
  bool put( signed int val )      { return put( &val, sizeof(val) ); }
  bool put( unsigned int val )    { return put( &val, sizeof(val) ); }
  bool put( signed long val )     { return put( &val, sizeof(val) ); }
  bool put( unsigned long val )   { return put( &val, sizeof(val) ); }
  bool put( gint64 val )          { return put( &val, sizeof(val) ); }
  bool put( guint64 val )         { return put( &val, sizeof(val) ); }
  bool put( float val )           { return put( &val, sizeof(val) ); }
  bool put( double val )          { return put( &val, sizeof(val) ); }
  bool put( bool val )            { return put( &val, sizeof(char) ); }


  bool get( signed char &val )    { return get( &val, sizeof(val) ); }
  bool get( unsigned char &val )  { return get( &val, sizeof(val) ); }
  bool get( signed short &val )   { return get( &val, sizeof(val) ); }
  bool get( unsigned short &val ) { return get( &val, sizeof(val) ); }
  bool get( signed int &val )     { return get( &val, sizeof(val) ); }
  bool get( unsigned int &val )   { return get( &val, sizeof(val) ); }
  bool get( signed long &val )    { return get( &val, sizeof(val) ); }
  bool get( unsigned long &val )  { return get( &val, sizeof(val) ); }
  bool get( gint64 &val )         { return get( &val, sizeof(val) ); }
  bool get( guint64 &val)         { return get( &val, sizeof(val) ); }
  bool get( float &val )          { return get( &val, sizeof(val) ); }
  bool get( double &val )         { return get( &val, sizeof(val) ); }
  bool get( bool &val )           { val = false; return get( &val, sizeof(char) ); }

  //.......................................................
public: // обработка строк 
  //.......................................................

  // Записать строку в поток
  // Схема записи: <длина><строка>
  // st - сохраняемая строка
  // len - длина строки (не более 0xFFFF). Если len = -1, то вычисляется автоматически.
  // Результат: данные записаны в полном объеме
  bool writeStr( const char* st, int len = -1 );

  // Прочитать строку из потока
  // buf - буфер для чтения (должен быть достаточен для размещения строки)
  // sizebuf - размер буфера
  // Результат: размер строки или -1 в случае отказа
  int readStr( char *buf, unsigned sizebuf );

  // Записать.прочитать строку в поток как и writeStr/readStrS, но длина строки не более 0xFF
  bool writeStrS( const char* st, int len = -1 );
  int readStrS( char *buf, unsigned sizebuf );

  // Прочитать строку из потока
  // Результат: прочитанная и размещенная в динамической памяти строка. Освобождать память функцией g_free
  gchar* readStr();

  // Вывод в поток строки и добавление в поток символов перевода строки 
  // под Win32 - "\r\n", иначе - "\n"
  bool writeStringEOL( const char* st, int len = -1 );

  // Вывод в поток форматированной строки
  bool printf( const char* format, ... );
  bool vprintf( const char* format, va_list arglist );

  //.......................................................
public: // навигация по потоку
  //.......................................................

  virtual long curPos( );             // текущая позиция указателя или -1
  virtual bool goTop( );              // указатель в начало файла
  virtual bool goPos( long pos );     // указатель в заданную позицию
  virtual bool goBottom( );           // указатель в конец файла
  virtual bool shiftPos( long delta );// сместить указатель

  //.......................................................
public: // управление совместным доступом к потоку
  //.......................................................
  //TIOStream* ref();             // Получить ссылку на поток
  //void unref();                 // Разорвать ссылку на поток
  //void close() { unref(); }     // закрыть поток

  //// Управление флагом закрытия файла при сбросе кол-ва ссылок в 0 (по умолчанию = true)
  //bool closeOnUnref();
  //void closeOnUnref( bool flag );

  //.......................................................
public: // прочие методы
  //.......................................................
  virtual bool isOpen() const;      // открыт ли доступ к потоку
  virtual long length();            // длина файла
  virtual bool flush();             // сбросить буфера в поток
  virtual bool chsize( long size ); // установить размер файла

protected:
};

//---------------------------------------------------------------

// Вывод в поток форматированной строки
inline bool TIOStream::printf( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  bool res = vprintf( format, arglist );
  va_end( arglist );
  return res;
}

//---------------------------------------------------------------

#endif // IOStreamH
