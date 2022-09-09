/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef GlobalParamsH
#define GlobalParamsH

//#define DURATION_FIGHT_MINUTE 15 // ������������ ���,�����
//#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // ������������ ���, ��
//#define COUNT_COMMAND_IN_FIGHT 1// ������ ����� �� ������� � ���
// \\ ������� ��������� \\

//#define SERVER_NUMBER_OF_NETWORK 0 // ??  ����� ������� (� ������ ������� ���������� ������� ���������)

// ������
#define VER_GAME_ENGINE           ((unsigned short)41)
#define COUNT_GAME_ENGINE_MODULES ((unsigned short)2)

#define VER_GRAPHIC_ENGINE ((unsigned short)56)
#define VER_PHYSIC_ENGINE  ((unsigned short)0)
#define VER_NET_ENGINE     ((unsigned short)21)

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
