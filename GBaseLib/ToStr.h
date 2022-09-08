#ifndef ToStrH
#define ToStrH

#include "TypeDef.h"

//------------------------------------------------------------------------------
// Класс символьного представления числовых значений с разделением на триады (по 3 разряда)
class GBASELIB_EI TN2A
{
  char thousands_sep;	// символ-разделитель числа по тетрадам
  char decimal_point;	// символ-разделитель дробной части числа
  bool isInit;	// библиотека инициализирована
  
  inline void init() { if( !isInit ) getSystemParam(); }
public:  
  // Получить системные параметры формирования
  void getSystemParam();

  // Получить символ-разделитель групп десятичных разрядов числа на триады
  // Символ возвращается в локальной кодировке
  char thousandsSep();

  // Установить символ-разделитель триад в разрядах числа
  // Результат - предыдущее значение
  char thousandsSep( char value );

  // Получить символ-разделитель дробной части числа
  char floatSep();

  // Установить символ-разделитель дробной части числа
  // Результат - предыдущее значение
  char floatSep( char value );

  // Удаление триадных разделителей из строки
  // src - исходная строка
  // dst - буфер под результирующую строка. 
  //       Если не задан, то переписывается исходная строка. 
  // Результат - "очищенная" результирующая строка
  char* clear( const char *src, char *dst = NULL );

public:

  // представления числовых значений с разделением на триады
  // st  - буфер под строку
  // val - числовое значений
  // Результат: строка с результатом (=st)
  char* conv( char* st, int val );
  char* conv( char* st, guint32 val );
  char* conv( char* st, gint64 val );
  char* conv( char* st, guint64 val );
  // cntFloat - кол-во дробных разрядов
  char* conv( char* st, double val, int cntFloat );

};
//------------------------------------------------------------------------------
// Глобальный объект форматирования чисел
extern GBASELIB_EI TN2A N2A;

//------------------------------------------------------------------------------
// Получить символ-разделитель групп десятичных разрядов числа на тетрады
// Символ возвращается в локальной кодировке
inline char ThousandsSep_Get()              { return N2A.thousandsSep(); }

// Установить символ-разделитель триад в разрядах числа
// Результат - предыдущее значение
inline char ThousandsSep_Set( char value )  { return N2A.thousandsSep( value ); }

// Получить символ-разделитель дробной части числа
inline char FloatSep_Get()                  { return N2A.floatSep(); }

// Установить символ-разделитель дробной части числа
// Результат - предыдущее значение
inline char FloatSep_Set( char value )      { return N2A.floatSep(value); }

// Удаление триадных разделителей из строки
// src - исходная строка
// dst - буфер под результирующую строка. 
//       Если не задан, то переписывается исходная строка. 
// Результат - "очищенная"  строка
inline char* ThousandsSep_Clear( const char *src, char *dst = NULL ) { return N2A.clear(src, dst); }

//------------------------------------------------------------------------------
// представления числовых значений с разделением на триады
// st  - буфер под строку
// val - числовое значений
// Результат: строка с результатом (=st)
inline char* n2a( char* st, int val )       { return N2A.conv( st, val ); }
inline char* n2a( char* st, guint32 val )   { return N2A.conv( st, val ); }
inline char* n2a( char* st, gint64 val )    { return N2A.conv( st, val ); }
inline char* n2a( char* st, guint64 val )   { return N2A.conv( st, val ); }
// cntFloat - кол-во дробных разрядов
inline char* n2a( char* st, double val, int cntFloat )  { return N2A.conv( st, val, cntFloat ); }
//------------------------------------------------------------------------------

#endif
