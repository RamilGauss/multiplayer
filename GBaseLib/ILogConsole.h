///////////////////////////////////////////////////////////
//  ILogConsole.h
//  Implementation of the Interface ILogConsole
//  Created on:      29-май-2010 19:28:11
//  Original author: KIT
///////////////////////////////////////////////////////////

#if !defined(EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_)
#define EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_

#include "GCS.h"

class GBASELIB_EI ILogConsole : public GCS
{
public:
  ILogConsole() {}
  virtual ~ILogConsole() {}
  
  // Вывести строку на консоль
	// str - отображаемая строка
  // len - длина строки (если = -1, то вычислить ее strlen)
  // eol - флаг необходимости перевода курсора на новую строку после вывода
  virtual void write(const char* str, int len = -1, bool eol = true ) = 0;

  // название консоли
  // Уникальное среди консолей проекта имя, позволяющее управлять её поведением и 
  // связями на уровне конфигурационного файла
	virtual const char* name() const = 0;
};

#endif // !defined(EA_8E69BCFA_17EA_4797_A94F_221DE2574BF5__INCLUDED_)
