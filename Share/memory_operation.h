/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MEMORY_OPERATIONH
#define MEMORY_OPERATIONH

#include "TypeDef.h"

/*
  mo_realloc_XXX - �������� ������ � ����������� ���� ���������� ������ ������. 
               ���� ������ ����� ������ ����� ������ ��� ������, �� ����������� ����� ��������.
  
  mo_realloc_bound_XXX - ���������� ������� ������ ����� ������� ����� ������ � ������������ 
               ���� �� �������� size_bound.
               ����� ������(�� ��� ��������) ����� ��������� �����. � ��������� ������ ����� �����
               ������ ������.
*/


// ����� malloc � free
extern SHARE_EI void* mo_realloc(void* old_mem, int old_size, int new_size);

extern SHARE_EI char* mo_realloc_bound(char* old_mem, int old_size, int size_bound, int size_paste);

// ����� new � delete
extern SHARE_EI void* mo_realloc_new(void* old_mem, int old_size, int new_size);

extern SHARE_EI char* mo_realloc_bound_new(char* old_mem, int old_size, int size_bound, int size_paste);

#endif