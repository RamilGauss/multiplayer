/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef GlobalParamsH
#define GlobalParamsH

//#define DURATION_FIGHT_MINUTE 15 // ������������ ���,�����
//#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // ������������ ���, ��
//#define COUNT_COMMAND_IN_FIGHT 1// ������ ����� �� ������� � ���
// \\ ������� ��������� \\

//#define SERVER_NUMBER_OF_NETWORK 0 // ??  ����� ������� (� ������ ������� ���������� ������� ���������)

// ������
#define VER_GAME_ENGINE           ((unsigned short)42)
#define COUNT_GAME_ENGINE_MODULES ((unsigned short)3)

#define VER_GRAPHIC_ENGINE ((unsigned short)56)
#define VER_PHYSIC_ENGINE  ((unsigned short)0)
#define VER_NET_ENGINE     ((unsigned short)22)

extern char strVerGameEngine[260];
extern char strVerGraphicEngine[260];
extern char strVerPhysicEngine[260];
extern char strVerNetEngine[260];

/* �������� ��������� �������� ������� � ����
//----------------------------------------------------
//    ������ ���� � �������
//----------------------------------------------------
 ������ ����:
 ---------
 [MAIN]
    cnt=N
 [PART0]
    id=ID
    path=PATH
    ...
 [PART(N-1)]
    ...
 ---------
 path - �������� �����, ������� ��������� � ��� �� �����, ��� � ���� ������

 //----------------------------------------------------
 //    ���� �����
 //----------------------------------------------------
 ---------
 [MAIN]
    cnt=N
    common property - ����� ����������
 [PART0]
    id_model=
    id_behavior=
    coord=;;;
    orient=;;;
    state=
    ...
 [PART(N-1)]
    ...
 ---------
 ������ - �� �� ID �� �����
 //----------------------------------------------------
 //    ���� ������ DX
 //----------------------------------------------------
 ---------
 [MAIN]
    LOD= - ��������� �� ������ �� �������
    CntGroup=4 - ���-�� ������
 [JOINT]
    root=Hull - ��� �����
    node0 = Hull
    cntJoint = 5
    nameJoint0 = Turret
    matrix0_0=1;0;0;0;
    matrix0_1=0;1;0;0;
    matrix0_2=0;0;1;0;
    matrix0_3=0;0;0;1;
    nameJoint(cntJoint) = ...
    ...
    node1 = Turret
    ...
    node(CntGroup-1)
    ...
 [PART0]
    strPathShader=shader.fx - �������� �������
    strTechnique=TexturedSpecular - ������� ������ �������
    strTexture=PzVl_Tiger_I.dds - �������� ����� ��������
    strName=Hull - ��� (��� �������)
    vAmbient=0.4;0.4;0.4; - 
    vDiffuse=0.4;0.4;0.4;
    vSpecular=1;1;1;
    nShininess=1
    fAlpha=1.0
    bSpecular=0
    mTypeLOD=0   - �����������
    mflgNormal=1 - �������� ��
    primitives=Hull.obj - ����� ������, ��������, ��������
 [PART(N-1)]
 ...
 ---------

 
    ������ ������ ��� �����: 
  ����� Gun
  ����� Turret
  ������ Hull
  ����� TrackR � TrackL
  ����� ChassisR � ChassisL

 */

// ����� ������� � �������� �������� ��� ������ ������ ��������


#endif
