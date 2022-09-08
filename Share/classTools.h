///////////////////////////////////////////////////////////
//  classTools.h
//  ���������� ��� ����������� ��������� �������
//  Created on:      15-���-2009 09:53:00
//  Original author: �������� �.�.
///////////////////////////////////////////////////////////

#ifndef __CLASS_TOOLS_H__
#define __CLASS_TOOLS_H__

/**
 * ������, ������������ ����� ��� ������������ ����������� � 
 * ��� ��������� ������������ 
 */
#define NO_COPY_CLASS( classname )					\
	private:                                  \
    classname(const classname&);            \
    classname& operator=(const classname&);

#endif /* __CLASS_TOOLS_H__ */
