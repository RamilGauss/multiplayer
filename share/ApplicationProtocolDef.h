/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ApplicationProtocolDefH
#define ApplicationProtocolDefH

// ���� �������
//-----------------------------------------------------------------------------
// REQUEST  - R

#define APPL_TYPE_BASE_REQUEST             0
#define APPL_TYPE_R_TRY_CONNECT_TO_SERVER  (APPL_TYPE_BASE_REQUEST+0) // ������� ����������� 
#define APPL_TYPE_R_FIRE                   (APPL_TYPE_BASE_REQUEST+1) // ������� ��������, �� ���� ��������� ����� �� �����������
#define APPL_TYPE_R_IN_FIGHT               (APPL_TYPE_BASE_REQUEST+2) // ������ �� ���������� � ���

#define APPL_TYPE_R_GET_LIST_TANK          (APPL_TYPE_BASE_REQUEST+3) // ���� ������ ������

#define APPL_TYPE_R_EXIT_WAIT              (APPL_TYPE_BASE_REQUEST+4) // ������ ����� �� �������� ���

#define APPL_TYPE_R_CORRECT_PACKET         (APPL_TYPE_BASE_REQUEST+5) // ������ �� �������������� �����

#define APPL_TYPE_R_EXIT_FIGHT             (APPL_TYPE_BASE_REQUEST+6) // ������ ����� �� ���
//-----------------------------------------------------------------------------
// COMMAND - C

#define APPL_TYPE_BASE_COMMAND            	100

#define APPL_TYPE_C_KEY_EVENT             	(APPL_TYPE_BASE_COMMAND+0)

#define APPL_TYPE_�_CHOOSE_TANK             (APPL_TYPE_BASE_COMMAND+1) // ����� ����� �� ������

// reserved
#define APPL_TYPE_C_DISCONNECT_FROM_SERVER  (APPL_TYPE_BASE_COMMAND+2) // ������������� 

//-----------------------------------------------------------------------------
// ANSWER - A

#define APPL_TYPE_BASE_ANSWER             200
// Client protocol (Client GUI)
#define APPL_TYPE_A_TRY_CONNECT_TO_SERVER (APPL_TYPE_BASE_ANSWER+0) // ������� ����������� 
#define APPL_TYPE_A_ECHO                  (APPL_TYPE_BASE_ANSWER+1) // ���,��������
#define APPL_TYPE_A_IN_FIGHT              (APPL_TYPE_BASE_ANSWER+2) // ����� �� ������ ����� � ���
#define APPL_TYPE_A_GET_LIST_TANK         (APPL_TYPE_BASE_ANSWER+3) // ���� ������ ������

#define APPL_TYPE_A_EXIT_WAIT             (APPL_TYPE_BASE_ANSWER+4) // ����� �� ������� ����� �� ������� �� ��������
#define APPL_TYPE_A_EXIT_FIGHT            (APPL_TYPE_BASE_ANSWER+5) // ����� �� ������� ����� �� ���

#define APPL_TYPE_A_END_FIGHT             (APPL_TYPE_BASE_ANSWER+6) // ����� �����, ��������� ���

// Graphic Engine (BigJack)

#define APPL_TYPE_G_A_FIRE_RELOAD                 (APPL_TYPE_BASE_ANSWER+7) // � ����� �� �������, ����� �����������
// � ����� ���� ������� ���, ����� ���� ��������� ������� � ����� ������:
#define APPL_TYPE_G_A_CORRECT_PACKET_STATE_OBJECT (APPL_TYPE_BASE_ANSWER+8)  // �������������� �����, �� ������� �������� DX
#define APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK   (APPL_TYPE_BASE_ANSWER+9)  // �������������� �����, �� ������� �������� DX

#define APPL_TYPE_G_A_SCORE                       (APPL_TYPE_BASE_ANSWER+10)  // � �������������� ����� � ������ �����
// ��� ����� �� ������ ������ - ������� �� ����
#define APPL_TYPE_G_A_EVENT_IN_FIGHT              (APPL_TYPE_BASE_ANSWER+11) // �������, �������� �� ������

//-----------------------------------------------------------------------------
// STREAM - S
// ���������� � ������, ��������

#define APPL_TYPE_BASE_STREAM           300
// Client protocol (Client GUI)

#define APPL_TYPE_S_GARAGE              (APPL_TYPE_BASE_STREAM+0) // ����������: �������� � ��� � ����� ���-��
#define APPL_TYPE_S_WAIT                (APPL_TYPE_BASE_STREAM+1) // ����������: ���-�� ��������� � �������, � ��� � ����� ���-��

// Graphic Engine (BigJack)
// ������ ������ ����� ���������
#define APPL_TYPE_G_S_LOAD_MAP            (APPL_TYPE_BASE_STREAM+2) // �� ����� �������� ����� �� �������
#define APPL_TYPE_G_S_COUNT_DOWN          (APPL_TYPE_BASE_STREAM+3) // ���������� ��������� ������� � ������ ���
#define APPL_TYPE_G_S_FIGHT_COORD_BULLET  (APPL_TYPE_BASE_STREAM+4) // ���������� � �������� ������, ��������

#define APPL_TYPE_S_ORIENT_AIM          (APPL_TYPE_BASE_STREAM+5) 
// �������� � ������� 10 ��� � �������, ���� �� ���������� �������, ���������� 3 ����: ���������, ����������, ������� �������
// ���������� � ��������� �������� �������, � ��� ������� ������ ������ ����(��������) ��������� �������� �������� ����������.
// �� ���� � ������� �������� ���� ���������� ��� ������� �������, �� ������� ��� ����� (�� ����������, � ������� ��������� ��������� ������ (��������� ����� ��� ����))
//-----------------------------------------------------------------------------
#endif