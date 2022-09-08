#ifndef TObjectH
#define TObjectH

#include "BL_IOStream.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

DECLARE_CLASS( Object )

// Базовый класс иерархии
class GBASELIB_EI TObject
{
public:
  TObject() {}
  TObject( const TObject& ) {}
  virtual ~TObject() {}

  // Сравнить себя на равенство объекту <obj>
  virtual bool operator==( const TObject& ) const
    { return false; }

  // Сохранить объект в поток
  virtual bool save( TIOStream& ) const
    { return true; }

  // Загрузить объект из потока
  virtual bool load( TIOStream& )
    { return true; }

  // Вычислить размер сохраняемой части объекта
  // Результат: кол-во байт
  virtual int length() const
    { return 0; }

  // Создать экземпляр объекта своего типа
  virtual PObject dubl() const
    { return new TObject (); }

  // Скопировать в себя объект <obj>
  virtual bool Assign( const TObject& )
    { return true; }

};

#endif
