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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef GlobalParamsTankH
#define GlobalParamsTankH

#define ServerLocalPort 4321

#define VERSION_PROGRAMM ((unsigned short)44) // unsigned short
#define VERSION_EDITOR   ((unsigned short)7)
//-------------------------------------------------------------------------------------------
extern char strVersionClient[100];
extern char strVersionServer[100];
extern char strVersionEditor[100];
//-------------------------------------------------------------------------------------------

#define STR_VERSION_CLIENT strVersionClient
#define STR_VERSION_SERVER strVersionServer
#define STR_VERSION_EDITOR strVersionEditor


#define DURATION_FIGHT_MINUTE 15 // ������������ ���,�����
#define DURATION_FIGHT_MS (DURATION_FIGHT_MINUTE*60*1000) // ������������ ���, ��
#define COUNT_COMMAND_IN_FIGHT 1// ������ ����� �� ������� � ���


namespace nsCallBackType
{
	// ���� callback �������
	enum{
		eRcvPacket  = 0,		   
		eRcvStream  = 1,
		eDisconnect = 2,
	};
}

#define NAME_TITTLE "������ \"Tanks\""
#define Q_MESSAGE(sError) \
QMessageBox::warning( NULL, QObject::tr(NAME_TITTLE),sError );

#define SERVER_NUMBER_OF_NETWORK 0 //  ����� ������� (� ������ ������� ���������� ������� ���������)


#define QEVENT_PACKET             (QEvent::Type)(QEvent::User+0)
#define QEVENT_STREAM             (QEvent::Type)(QEvent::User+1)
#define QEVENT_DISCONNECT         (QEvent::Type)(QEvent::User+2)



#define PATH_LIST_MODELS "..\\model\\listModel.ini"
#define PATH_LIST_MAP "..\\map\\listMap.ini"


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

    cntJoint=N
    nameJoint0=Turret
    matrix0_0=;;;;
    matrix0_1=;;;;
    matrix0_2=;;;;   
    matrix0_3=;;;;
    nameJoint1=TrackL
    matrix1_0=;;;;
    matrix1_1=;;;;
    matrix1_2=;;;;   
    matrix1_3=;;;;
    nameJoint2=TrackR
    matrix2_0=;;;;
    matrix2_1=;;;;
    matrix2_2=;;;;   
    matrix2_3=;;;;
    nameJoint1=ChassisL
    matrix1_0=;;;;
    matrix1_1=;;;;
    matrix1_2=;;;;   
    matrix1_3=;;;;
    nameJoint2=ChassisR
    matrix2_0=;;;;
    matrix2_1=;;;;
    matrix2_2=;;;;   
    matrix2_3=;;;;

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
