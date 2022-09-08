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
#define APPL_TYPE_A_TRY_CONNECT_TO_SERVER (APPL_TYPE_BASE_ANSWER+0) // ������� ����������� 
#define APPL_TYPE_A_ECHO                  (APPL_TYPE_BASE_ANSWER+1) // ���,��������
#define APPL_TYPE_A_FIRE_RELOAD           (APPL_TYPE_BASE_ANSWER+2) // � ����� �� �������, ����� �����������
#define APPL_TYPE_A_IN_FIGHT              (APPL_TYPE_BASE_ANSWER+3) // ����� �� ������ ����� � ���
#define APPL_TYPE_A_GET_LIST_TANK         (APPL_TYPE_BASE_ANSWER+4) // ���� ������ ������

#define APPL_TYPE_A_EXIT_WAIT             (APPL_TYPE_BASE_ANSWER+5) // ����� �� ������� ����� �� ������� �� ��������
#define APPL_TYPE_A_EXIT_FIGHT            (APPL_TYPE_BASE_ANSWER+6) // ����� �� ������� ����� �� ���

#define APPL_TYPE_A_END_FIGHT             (APPL_TYPE_BASE_ANSWER+7) // ����� �����, ���� �� ���� ��� ���������� (��������� ���, ���)

// � ����� ���� ������� ���, ����� ���� ��������� ������� � ����� ������:
#define APPL_TYPE_A_CORRECT_PACKET_STATE_OBJECT  (APPL_TYPE_BASE_ANSWER+8)  // �������������� �����, �� ������� �������� DX
#define APPL_TYPE_A_CORRECT_PACKET_STATE_TANK    (APPL_TYPE_BASE_ANSWER+9)  // �������������� �����, �� ������� �������� DX

#define APPL_TYPE_A_SCORE                        (APPL_TYPE_BASE_ANSWER+10)  // � �������������� ����� � ������ �����
// ��� ����� �� ������ ������ - ������� �� ����
#define APPL_TYPE_A_EVENT_IN_FIGHT               (APPL_TYPE_BASE_ANSWER+11) // �������, �������� �� ������

//-----------------------------------------------------------------------------
// STREAM - S
// ���������� � ������, ��������

#define APPL_TYPE_BASE_STREAM           300
#define APPL_TYPE_S_GARAGE              (APPL_TYPE_BASE_STREAM+0) // ����������: �������� � ��� � ����� ���-��
#define APPL_TYPE_S_WAIT                (APPL_TYPE_BASE_STREAM+1) // ����������: ���-�� ��������� � �������, � ��� � ����� ���-��
// ������ ������ ����� ���������
#define APPL_TYPE_S_LOAD_MAP            (APPL_TYPE_BASE_STREAM+2)// �� ����� �������� ����� �� �������
#define APPL_TYPE_S_COUNT_DOWN          (APPL_TYPE_BASE_STREAM+3) // ���������� ��������� ������� � ������ ���
#define APPL_TYPE_S_FIGHT_COORD_BULLET  (APPL_TYPE_BASE_STREAM+4) // ���������� � �������� ������, ��������

#define APPL_TYPE_S_ORIENT_AIM          (APPL_TYPE_BASE_STREAM+5) 
// �������� � ������� 10 ��� � �������, ���� �� ���������� �������, ���������� 3 ����: ���������, ����������, ������� �������
// ���������� � ��������� �������� �������, � ��� ������� ������ ������ ����(��������) ��������� �������� �������� ����������.
// �� ���� � ������� �������� ���� ���������� ��� ������� �������, �� ������� ��� ����� (�� ����������, � ������� ��������� ��������� ������ (��������� ����� ��� ����))
//-----------------------------------------------------------------------------
#endif