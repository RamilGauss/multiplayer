///////////////////////////////////////////////////////////
//  classTools.h
//  Инструмены для модификации поведения классов
//  Created on:      15-окт-2009 09:53:00
//  Original author: Аршанкин Ю.Г.
///////////////////////////////////////////////////////////

#ifndef __CLASS_TOOLS_H__
#define __CLASS_TOOLS_H__

/**
 * Макрос, определяющий класс без конструктора копирования и 
 * без оператора присваивания 
 */
#define NO_COPY_CLASS( classname )					\
	private:                                  \
    classname(const classname&);            \
    classname& operator=(const classname&);

#endif /* __CLASS_TOOLS_H__ */
