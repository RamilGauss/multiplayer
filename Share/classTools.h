/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef __CLASS_TOOLS_H__
#define __CLASS_TOOLS_H__

/**
 * ������, ������������ ����� ��� ������������ ����������� � 
 * ��� ��������� ������������ 
 */
#define NO_COPY_OBJECT( classname )					\
	private:                                  \
    classname(const classname&);            \
    classname& operator=(const classname&);

#endif /* __CLASS_TOOLS_H__ */
