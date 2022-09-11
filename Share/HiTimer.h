/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef HiTimerH
#define HiTimerH

#include "TypeDef.h"

//------------------------------------------------------------------------------
// ������ ������ � �������� �������� ����������
//------------------------------------------------------------------------------

// ��������� ����� ���������� (� �������� �������� ����������)
unsigned __int64 SHARE_EI ht_GetUSCount();

// �������� �� ������������
void SHARE_EI ht_usleep( unsigned int us );

// �������� �� ������������
void SHARE_EI ht_msleep( unsigned int ms );

// �������� �� ������������ c �������������� ��������� ���������
// ���������: true - ����� �� ���������� func, false - ����� �� ��������
typedef bool (*THT_CheckFunc)(void);
bool SHARE_EI ht_sleep( unsigned int us, THT_CheckFunc func );

// ����� � ������������� � ������� ������� ��� ��� ���������.
// �������� �� 55 ���� � ����
unsigned int SHARE_EI ht_GetMSCount();
//------------------------------------------------------------------------------

#endif
