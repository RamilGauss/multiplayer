/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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

// ������� ��������� //
//#define ServerLocalPort 4321
//
//#define VERSION_PROGRAMM       ((unsigned short)55) 
//#define VERSION_VIEWER_MODEL   ((unsigned short)33)
//#define VERSION_CONVERTER_MESH ((unsigned short)1000)
////-------------------------------------------------------------------------------------------
//extern char strVersionClient[100];
//extern char strVersionServer[100];
//extern char strVersionViewerModel[100];
//extern char strVersionConverterMesh[100];
////-------------------------------------------------------------------------------------------
//
//#define STR_VERSION_CLIENT         strVersionClient
//#define STR_VERSION_SERVER         strVersionServer
//#define STR_VERSION_VIEWER_MODEL   strVersionViewerModel
//#define STR_VERSION_CONVERTER_MESH strVersionConverterMesh
//
//#define NAME_GAME "TankS"
//
//#define DURATION_FIGHT_MINUTE 15 // ������������ ���,�����
//#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // ������������ ���, ��
//#define COUNT_COMMAND_IN_FIGHT 1// ������ ����� �� ������� � ���
// \\ ������� ��������� \\

//namespace nsCallBackType
//{
	// ���� callback �������
	//enum{
		//eRcvPacket  = 0,		   
		//eRcvStream  = 1,
		//eDisconnect = 2,
	//};
//}
//#define SERVER_NUMBER_OF_NETWORK 0 // ??  ����� ������� (� ������ ������� ���������� ������� ���������)

// ������
#define VER_GAME_ENGINE           ((unsigned short)40)
#define COUNT_GAME_ENGINE_MODULES ((unsigned short)2)

#define VER_GRAPHIC_ENGINE ((unsigned short)55)
#define VER_PHYSIC_ENGINE  ((unsigned short)0)
#define VER_NET_ENGINE     ((unsigned short)20)

extern char strVerGameEngine[260];
extern char strVerGraphicEngine[260];
extern char strVerPhysicEngine[260];
extern char strVerNETEngine[260];

//#define STR_VER_GAME_ENGINE    ((unsigned short)40)
//#define STR_VER_GRAPHIC_ENGINE ((unsigned short)55)
//#define STR_VER_PHYSIC_ENGINE  ((unsigned short)0)
//#define STR_VER_NET_ENGINE     ((unsigned short)20)

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
