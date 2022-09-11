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


#ifndef DateChangesH
#define DateChangesH

// ���� �������� 1.12.2011

//***
//�� ����������� ��. ./doc/�����������.doc
//***

/*
    DONE
    TODO
    INFO
    BUG
*/


/* 
    DONE: 

21.12.2011 :
 - TList. ������ � �����������, ��� �������� ��� ���������� �� ������ �������.
 - ������� ����������.
25.12.2011 :
 - ������ ����� �� �������.
 - ��������� ������� ����� �������� � ��������.
 - ����������� � ������� ����������.
26.12.2011 :
 - ������ ��� ������� �����������.
27.12.2011 :
 - ��������� �������� �������� ������.
 - �������� �����������.
28.12.2011:
 - ����������� ����� ������ �������� � ���� �������.
30.12.2011:
 - ������: ������ �������� ��������� � TArrayObject ��� ��������� ��������.
 - �������� ������ ��� ������ ���������� synchro.
 - ��� ���� CN - ����� � �������� � ����������.
06.01.2012:
 - ����������� � ������ Stream � Packet.
07.01.2012:
 - �������� ���������� - ������ �� ������� � �������, 
 - ������: ������ �� 1. ���������� ������� �� ������� 3 ������� � 2.�� ������� Disconnect �� ����������.
 - ������: ������ �� ���������� ������� �� ������� 1 ������ -> 1.������� Disconnect �� ����������. ������ ������� ��� � ������ ���.
 - ����� ��� ������.
 - ����� ��� ������� �������� �������: ������� � �����. ��� � ������ ������� ���.
 
10.01.2012:
 - ������� ����������
 - ��������� ���������� �������� � ������� �������.
 - ��� ������.
 - ��� ������� �� ����.
11.01.2012:
 - ������������ ���������� � 220 ���������.+
 - ������������ ���������� � 450 ���������.+

13.01.2012:
 - ����, �������� ��������� :(. �������� � ����������������.
   ����������: ������ ���������� �� ������������ ���������� ������, �� �� ������� �� ������������.
               ������ ��������: �������� �� ����������, �� ������� �� ������������.
               ������� �� ��������� �������� ����� ����� ���������� �� ���� �������.
               �� 

14.01.2012:
 - ��� ���������� , �������.
 - ������ ��������� ����������� � ������ ��������� ������������� mArrWaitSend � mArrWaitCheck.
 - ������� �� �������� � ����������.
15.01.2012:
 - �������: ����� � ���� �������� �������� �������� ������ �������.
16.01.2012:
 - �������� ������ ������ � ������.
17.01.2012:
 - ����� ����� � ������.
18.01.2012:
 - ����� �������� ��� � ������ �� ���.
 - ������ �� ��� ��������� � ������������ �������.
 - ������� ���������.
19.01.2012:
 - ����� ��� ������ ���
21.01.2012:
 - ����� ��� ������ � �������, ������� � ����. ��� ��� ���� ���-�� �������.
25.01.2012:
 - �������� ������ ����� ���� �� �������.
 - �������� ������ ����� ���� �� �������.
12.04.2012:
 - DXUT + DirectX + Qt4
14.04.2012:
 - ����� ���������� ����������� �� ������ DirectX
15.04.2012:
 - ������� DirectX � Qt4 ��� ������� - ��� ��� ������ TBaseDirectX � ��������� ��������� � �������.
17.04.2012:
 - ����������� �������� ������� ������-������ � ������ ����� �������� ���������. � ����� ��� ��� �� 2-3.
 - ��������� Git. ��� ����������� ���� ��� ����. ���� ��������. �� ���� ������. �����, ����� ����������.
18.04.2012:
 - �������������� ����� ������������ ������� �� ����� 5-10 ������� � ������. ������ ���������� � TransportMain
 � ������������� �� ���-�� �������.
 - �������� ��� ����� ����������������� ������ DX � Transport Main ��� ������ �������������� ������� � ������.
 ����� �������� ��������������� ��������� �������� MultiThreadQueue. - 
19.04.2012:
 - �������� ClientMain +
 - �������� GameRoomPrepare +
 - �������� WaitForm +
20.04.2012:
 - �������� ManagerGUI. �� ��������� ������ �������. ����� ��������� ��������� ����������� ��� ���������� ��� � ������������ ������ �� �������.
21.04.2012:
 - �������� ����� � Queue.
22.04.2012:
 - ������� � TA_In_Fight. ������ ����� sNick � cnt Tank.
28.04.2012:
 - ���������� TA_In_Fight. ���� ����� ����� ��� ���������������.
01.05.2012:
 - �������. �� ��� ���� �������� �� ������. ������, ��� �� ������ ������� ����� �����. ����������� �����.
02.05.2012:
 - ���, �����.
03.05.2012:
 - ����������� DirectX.
 - ������ ��������. DirectX � ��������� ������������ �������.
 - ������ ������ � �������.
 - ������� ������� ��������������� ������.
05.05.2012:
 - ��������� TRoom.
09.05.2012:
 - ������� ���������(��� ����� ��������). ����� ��� ��������.
10.05.2012:
 - ��������� �������� ���� � ������: ������ ������ ����� ������ ��-�� ���� ��� 
 CRT ������������ ������ � ������ ����� �������� �������. � ~TGarage().
 �������� ������ �������.
11.05.2012:
 - ������ ��� ModelDX, ManagerModelDX.
14.05.2012:
 - �������� EditorModel.
17.05.2012:
 - ������ ��� Editor. �������� ������ �� ini-������.
18.05.2012:
 - ��������� ����������� ������. ��������� ������ ������ :)
21.05.2012:
 - ����� ������ ���������� ��� ����������� �� primitives � x ������.
  ��������� ���� ���������� ��� ���� ������. ������ ���� ���������� ���� �� ����.
  3D Object Converter
  ������������ �� primitives � *.obj (WaveFront). ����� ���������� ��� ��������� � Mesh �����
  ������� �������� ��� �����������:
  1. �������� ������.
  2. Options->Export->WaveFront ��������� ������� Export Vertex Normals
  3. Tools->Flip Scene UV Map Vertically.
  4. Save as WaveFront.
  5. ���������� *.mtl � �������� �������� � ������ ����: 
                newmtl Material_1
                Ka 0.4 0.4 0.4
                Kd 0.587609 0.587609 0.587609
                Ks 0.171744 0.171744 0.171744
                illum 5
                Ns 8.000000
                map_Kd PzVl_Tiger_I.dds
 - ����������� �������. 
11.06.2012:
 - �������� SelfTank, ����� � ��������� ���������� ������ ManagerDirectX.
14.06.2012:
 - ������ ��� ��������� GPL.
 - Melissa - ���������, BigJack - �������, Robert - ������, ������(GUI) -- ��� ������� ��������� �����������
 - �������� ��������� ��� ���������: ������ �� ���������� ������ ��������� �������� � BigJack.
�.�. ��� ��� �� ��� ���������� ���������.
05.07.2012:
 - ��������� ����������� Qt + DirectX Qt4.5.0.
 - ����������� ������� � ������ �� ������ DXUT. �������� ����� Qt+DX.
19.07.2012:
 - �����������. ��������� ������������� ���������. ���������� � ������� ��������� ��������� �������.
ITBaseObject, TanksLib.lib, TManagerGUI, TBaseGUI_DX.
 - ����� ������� ��������� ������� OBJ. �� ���� EditorModel ���� �� �����.
07.08.2012:
 - ����� ����������� � Enterprise Architect.
08.08.2012:
 - ������ ������� � ������. � ����� ������������. (doc/����� �����������.EAP)
 - ������ v0.040
15.08.2012:
 - ����� �������� BigJack. �������� �������� IBaseObjectDX::SetModel - ���������� �������.
20.08.2012:
 - ������� TCallbackRegistrator, ������� std::set<...>.
29.08.2012:
 - ������� �� DXUT. ������ ����� Qt �������� � DXUT.
 - BigJack ���������. ���� ���� � ���, ��� Subset � DrawSubset ��� ����� 0, � ������ ���� ����� 1.
30.08.2012:
 - ������� � ������� *.obj �� *.bj, ����� ������ �������� ������� � ���������� �������� � ������ ����� ��������
 � 5 ������ �� 73 ��. *.bj - �������� ������ �������� Mesh. BigJack ������.
31.08.2012:
 - ���������� � ���������. ����� ��� ������� �������� ������ ������ ���� � ����� ��-���������.
03.09.2012:
 - ���� ��� ����������. ��� � WOT XZ ��������� �����, � Y ��� ������� ������ �����.
10.09.2012:
 - ��������: ���������� �������� �� ����� ������������� � "������".
16.09.2012:
 - �����,  ������ ������ �� ����������. ����� Tree �������.
 - ������ � ��� ����������.
20.09.2012:
 - ���� ������� ������ � ������� ������ � �����.
27.09.2012:
 - ��������� ��������� XML �������. ������, ������. ����� CMarkup.
 - ������������ ����� ������� �� Qt �� DXUT.
01.10.2012:
 - ������ �������� ��������. �������� ������� �� ��� ��������� ���� � ������. � ������ ����� ��������.
02.10.2012:
 - ��������� ����������� ���.
04.10.2012:
 - ������� ������� ������ ������ ������� �����������. �� ���������� �����.���� � ���, ��� 
 ������������ ������ �������� ������������ �������� �� ����������. ������ ���������� �����
 ����������� � ����� ��������� �������. ������ ���-�� ���������� �������� � ������
 ���������� (���� ������ �������� �� ����� �������). �������� �������� ����������� �� ���������� �� ������.
 �� ������ �������� �� ��������. ���� ����������� - ���� � �������.
31.10.2012:
 - ��������� �����������. ����: ������� ���������� Qt ����� (������ ���������� �������� WinApi 
 � ������� � Qt ����������, ����� ������� �������������� ManagerEvent). �.�. ����� ��������� 
 ������������� � GUI �������. � ����� ���������� ���������������� �� �� Qt, �, ��������, �� ������� GUI.

05.11.2012:
 - �������� ���� �� �������� Qt � DirectX. ���� ���� (����� ���, ���� ���������� �� ��� �������):
 1. ����� ����� �� DXUT ��������� ��� � QImage � ������ �� QWidget. ����� �������� GUI � ����������� Alpha. - ��� ����� ��� ����� ���������.
 2. �������� �������� GUI � ������ � DXUT. ��� ���������� DirectX ���������� �����-���������� � ������������� ������.
 ����� ����������:

06.11.2012:
 - Qt+DXUT = R.I.P.
 ����� �� ���������� ������� ������. �� WinXP 30 ��, � �� Win7 200 �� (!!!). �� ��� ��� ������� �������.
 ���� ������ ������. �� ��� �� �������� ��������. ����������� Qt->DXUT � �������� ������� ��������.
 ����� ������ ����  ������ - DXUT Native GUI. �� ������ ���� ��������� ����������� � ������ ����������
 �������. NET, GUI, LoadFromHDD, Key+Mouse � �.�. ���� ������ ����������(��) ������� ����. �� ����� ����������� ���������,
 �� ����� ��������� ��� ����������� ������� ������� �� ����� � ����������� �� �������� �����������. ���� ����������������
 ����� ������ � DGUI, �������� ��� ����������� �� � ����� � MOC. �� ������ ���� �������� ���.
08.11.2012:
 - ��� ���� ������� GUI DXUT ������� ����������� ����������. 
 - ����������� � ����������� GUI DXUT.
12.11.2012:
 - ���� ���� �������� TankLib(������, ������� ���������� ��� ���� �����), �� ������ � ���� ������������� ���.
� ������ ��� ������� �� Python � "������������" � �++ ������ � ��������� �����������.
�� ���� � �� ������ ��� ����������, ���� ��� ������ ������� � GameLib 
(��������, TMakerObject, ����� NewByID_Behavior()). ����� ����� ������ � Python.
21.11.2012:
 - ����� Python. ������� DLL. � ��� ��� ������� GetClientDeveloperTool(),
GetServerDeveloperTool() � Done(IDeveloperTool*). ������� ���������� �������, ��������� ������� ��������� � ������.
IClientDeveloperTool � IServerDeveloperTool. �� ������� ������ � �������������
Python. ��� ������� �� C++. ������ ���������� ������������ ������� � ������ ��������.
����� Developer ������ �������������� ��������� �������� �� �����(����������� �� IBaseObjectCommon) � 
������ �� �������� (IMakerObjectCommon). ��� ������, ������� ������� ��������� � �����, ����������
�� IActor. ����� ��� ������ � GUI Developer ����������� �� IGrahpicEngineGUI. � ������������
�������� Load(���� � XML). ����� Connect(���������,�������,����������).
 - ����� ���� � ����������: � IClientDeveloperTool ���� ����� int ConvertKeyEvent2Value(...) � 
int ConvertMouseEvent2Value(...). ��������, ���������� �� ���� ������� �������� 
�� ���������� �������. �������� ��������� �������� �� �������� ������ ����. ������ ���� ��� ���������
���������� � ���������� ���� ������.

23.11.2012:
 - ��. Architecture v0.4.eap � Architecture Included Headers.eap.
 - Game.exe -a s/c -p client.dll --p start with param ip=0.0.0.0 port=1000
 �.�. ������� ������ ����. ��������� ������. ��� �� ���:
 if(argv[1]=='s')
   new TClient
 else
   new TServer
25.11.2012:
  - ���� ���: Local, Online, Massive-Online. � ����� � Master-Server, Slave-Server � Client.
  Local - � ����� ��������, ��������� � ������ �������.
26.11.2012:
  - ��� �� �����! � �����������, � ������ ���������� �����-�� ������. ������ � ���������, �������� � 
�����������. �������� ����� ������������ �� ������ ������ ���������. ��� ����������� ����� ��� �����?
�������� ��� �� ��� ���� ������ ����� �����. "� ���� ����� ��������". ���, � �� ��� ������?
� ���� ���������� �����������. ������ ���� �� ������������� ����������? �� ��� ����� ����� ����� ���� �����
��������� �����? ���� ������� ����, �� �� ���� ����� ���� ������� ��� � ����? �� � ��� �� ��� �����?
� ���� �� ���� ������� � ����� ��� ����� ���������������, �� �� ��������� �����.
��� ����� ����� ��������? ���� �������������� � ������ �����. � �� ����� ����. 
��� ������� ���� � ���, ��� � ���� ������ �����������. ���� ������� ���� ����������
���� �� ���������� - ��� � �����. �� ���� ���� ���� ���� ��������? ��������� ����� ���� ��� 
���������� - �� ����������! � �����. � ���� ��� �����.
� � ��� ��� ����� �� �������� �� ����. � ������������� ����� ������ �� ����� ��������.
���� �������� �������� �� ��� ����. �� �� ��� ��� ��������. � �� � ������ ��� �� �����������.
��� ����� �� ��� ����� ���� ��� ������. ���??? �� ������ ������ �� �����. ��������� ��������� ���� ��� �� ���.
����� ��� �� ��� � ��� ���� �� �� ������ �������� � ��. � �� ���� ������ �� �������� ��� 
���������� �� ������ � ��� �����. �� ��������, ���� ��� ����� ����� �������� ����� ��� ���.
� ������� ��� ���� ������ �����.
  - Massive-Online - ������� MasterServer-SlaveServer. �����������. ������������� �������� � 
�������� ����� ����� �������������.
������ Master - 1. ��������� ����� ������� ��� ����������� ���������� � ����������.
                2. ������������� �������� ����� Slave.
                3. ����� �� ������� � MasterServer � SuperServer ���������� � �������� �� ��� ����.
                4. ������� � �������������.
  - ���� � ������������. ���� GUI � GE �������. ���������� �� ������� ��� ������ ���� ������������� 
  ������������ � ��� ��� ��� �����������.
27.11.2012:
  - ����� ������ � �������. ������������ ���� ����� �������������� ������ �������.
28.11.2012:
  - ��������� GameLib.lib : 
  1. Client-�������, 2. Client-������, 3. Slave-Master, 4. Slave, 5. Master, 6. SuperServer.
  + ���� � *.dll    
  - ������������� � ��������� :)
  - ������� ������ DLL.
29.11.2012:
  - �������� ������������ ������ ��������. ������� �������� ������� ���� ��������.
07.12.2012:
  - �����: 1. ������ ������ ���� � GameLib(����������). ICamera ��������� � Share. ���� � ������� 
  ������� ��� ���������� �������.
  2. � ������� ��� �������� - �������, ������ � ����.
  - ���� ���������� � ������� GUI. �� �����: MyGUI, CEGUI, Antisphere, librock, GWEN, Janella(���� 
  ���� �������� �����) � ��� ���� ������ �����.
08.12.2012:
  - ����� ������ ������� � �������� �� MyGUI. ��� �������� ���� ���������� ����������:
  1. ������� �������������� MyGUI_Engine.lib(dll). - �� 1 ������ �� 1 ������.
  2. ������� Platform. - 1 ������
  3. ������� BaseManager (��������� ��� ������ � MyGUI). 1-2 ���
  4. ������� ������ ��� ������������� ���� ��� ������ ����������� (������������). 1-2 ���
  �������� �� DXUT �� MyGUI ����� ���� �� 2-3 �����.
  - ������ ������ ������� ������. ������������ ����� � ���� �� ��������. ���� ���� ��� ��������� 
  ������ ��������� ������� ��������� ����������. ��������� ���� GUI.lib, �� ��� ��� ��������� �� 
  DXUT �� Platform.lib.
  - ����������� ����� ����� ��������  ����� 2-3 ����� (��� ��� ����� ����� �������� ����� �� ��� DXUT).
11.12.2012:
  - �� ��� ��� ����������� � ��������� MyGUI!
  - ��� �������� � �� �������� � ������� �� CEGUI.
  - �������� ��������� ��� ��� �������� � multiplayer.
14.12.2012:
  - �������� ������ ������. ������� ��� ��������� ������� ������� ������� ���� ������� (�� 12-15 ��)
�� �����. ���� �������� ��������� ������� ����� ��� 1000 fps.
  - ������� ����� �� Wrapper_MyGUI_Export. ��� ������� � ���� ���-�� ����.
  ������ ���� ���������� ��������� MyGUI. ����� ������.
15.12.2012:
  - �� ��� ��� ����� GUI. ��� ��������� ������� ��� ��������.
  �� ������� TManagerGUI. Static GetComponent.
19.12.2012:
  - ��������� GUI.lib � ������. �������������� solution.
20.12.2012:
  - ������� GUI. ���������� � ����� ���������, �� ����� ��.
  ���� ��� � ���: ���� ������������ ���������� ���������������� ���������� ���� lib
��� Exe � DLL, �� ��� ����� ��� ������ �������� ������������.
������� ����� ������������� MyGUI � ���� DLL.
  - ���� ���� ��������� - ��� �������� � full-screen ���������� ResizeGUI, �� �����
������� ResizeGUI �� ��������.
  - ������ - ���� � Tools GUI, resize, Camera, �������� �������, 
21.12.2012:
  - �������� ���� � ������������ ResizeGUI. ���� � ��� ��� DXUT �������� Reset �� ���������
��������� ������� ����. ��� ������� ��� � ������ ����. Reset ��� � ������ ���� �����.
������� DXUT � ��� ��� ��� ������� "������� � �� �������� ������ � ����� �������� � �������".
22.12.2012:
  - ����. ����� �������� ������ MyGUIEngine ��� ��� Dll-������. ����� � ��� ��������� � Lib.
� ��� EditorFramework ��� ���������� ��� Lib. ���� ���� �� ������ ��� �������, � ��������� ������.
� ������� ��������� define MYGUI_BUILD_DLL � ���.
25.12.2012:
  - ����� ������� ������� ������ FullMaster ��� Tornado.
  ����� - ��� ����� ���� ������ �������������. ���� ��������� ��������� ������.
26.12.2012:
  - ������� ����� �������� �� ���-��� ����������� �������� ������ ������.
  ������ ���� ����� ���������� ���-�� ����������� �������� ����� ����������������
  �� TOnly_N_Object � ������� � ������������ ����. ���-�� �������� � 
  �������� NAME_CLASS ��������� ��� ������.
  - �������� GBaseLib, Share �� Lib � Dll (���� ��� �� � ������ ������� �������������� � Dll).
  - ����� ����� ������� �����������.
27.12.2012:
  - ����� ������. ��� ����� ���� ����������� ������ ������ (�����������).
  �����������: �� ���� � ������ � DX � Struct3D ���� � ����.
  ��� ����� ��������� � ��������� ��� �������. � ��� ��������������� �� Struct3D � DX 
  � �������� �������� ���-�� ��� ���������.
09.01.2013:
  - ������� ��� ������ ���� TMakerXXX �� ������.
  - ���������� ������ �������������. ��� �������� ��������� ��������� ���, 
  ������ � ������-����������.
  - ����� ����� ��� "TornadoEngine"
11.01.2013:
  - �����-�� ���������� �� ������.
13.01.2013:
  - ������� orient � ���� ��������� � ����� ������. ���� ������� ��������� (list<>).
15.01.2013:
  - �������� �������� ������ - ��������� ��������� ���������� ������ TCamera. ����� ������ �����������.
  �����, ����� ��������� �� ������ ����� � ������� ��������� �, ���������� ����� ���������� ��������, 
  ������������� �������.
16.01.2013:
  - �������-�� �������� LookAt � ������. ������, ����� ������ ������� � �����, ����� ��������� Roll ������.
18.01.2013:
  - �������� ���� �����.
  - ������ ��������� ����� � �������� ������ (������).
29.01.2013:
  - �������� ������� �����. ��� �������� ����������(��������� �������) � HotKey.
  - ������ ��������� �����. ����� � ������� ��� �������� ������� ����������� ������.
  � ��� ������� ����� ������� ����� ���������� (������ ������). �����, ������� ������
  �� ����� � ������, ��������� ����������. � ������ ��������� ���-�� ����� 
  ������� Update().
30.01.2013:
  - ����� boost. ��� ���� ���, ��� �����. ��������� ��� ���������� ����������.
  ����� �������� TMapDual �� boost::bimap, TStateMachine �� boost::msm.
07.02.2013:
  - Client ����� � ����� ��������� ������� � ����������� "������". 
  ������-Slave � ����� ��������� ������ �� ����� (��������� ������ 
  �������������� ������ ����������� ������).
08.02.2013:
  - ������� � ������ �������� � �������, ����������� ��������� ������ � �������� ��������.
11.02.2013:
  - ����� ������-��������� ���������.
13.02.2013:
  - ������ �����������. �������� � ������ ������ ������������� �� ������� � �����.
  ����� 2 ������� ���� �������� �������� ���-�� ������ � ��������. ����� ������.
23.02.2013:
  - ������ ���������. �������� � ��� ������������ ������ ������ �� UDP 8�.
  ������ ���� ����������� ��������� ����� �������� ������, ����� ����� ������������ ���� �����-�������
  (��������� ������ � ������) ���� TCP �����.
  ��� ���������� �������� ������������ ������������� ������:
  ���� �������� ��� ��������� (����� ������ ������������ ��� �������� �������� ���������� �������),
  ���� ������������ ������������ (�������� �������� ���������� �������).
  ��� �������� ����� ����� ����� ��� �� ������� (���� �����) ��� � �� �������.
28.02.2013:
   - ���� ������ ���������. ������� ��������� ������������ ������ ���� ����������� ��������.
   ����� ��������� ������ � ���, ��������� ���������� � ���������� ����������. � �� ��������.
  "Master Of Puppets"

30.04.2013:
   - ���������� ���������� ������� ���������. ������ ���� ������� ������� ������� �� ������� � �������� ��
   Melissa.dll � NET_Transport.dll.
   - �� ����� ����� ���������� ����������� ������ �����������.
	 - ��� Melissa ���-���� �������� ������������ ������������ �����, ������� ���������� ��� ���������� Qt.
	 �� ���� ��� �������� ������� �� ����� �����������.

04.05.2013:
   - ��������� ������������� Share � GBaseLib (������� ����� Melissa):
		1. ������� �������������� �����.
		2. ������� ��������� ����� ������� ������������� � glib.
		3. ������������� ����������.
		4. ����������� ����� � ��������������� ���������� (Share<-->GBaseLib).
	 - �������� NET_Transport, Melissa � �������� DBLib (������� 2013).
07.05.2013:
   - ��� ������������ ���������� TNetTransport � ������� ����� ���� ��������� � ��������� �������
   FAST_NET_TRANSPORT. �� �������� �����������, ��� ����� ������� �������� ��������� �������.
   - �������� ������������� ������� ��� ��������� ������� �� ������� ��������� (����� 100 ����/�).
   �� ��� ��������, ������� �������� ������ ����� ��������� Melissa.
   ������� ��� 100 ���� 10 �� �������� � 150-250 ������� � ����� ������� (��� NetDoser ��� 
   ���������� ����� �������).
   - ���� ��� ������: ��� ������� ��������� ������ ������ �� ����� ������������� �������.
   ���� ������, �� ������ ����� � ���������� � ������� ���� ���, ��� �� �� ����������� ������ � ������.
   ���� ������ �� ������ ���������.
03.06.2013:
   ���������� ��������� �����
07.06.2013:
	 ����� ������� � ������� ����������� ������� � ������ ��� ����� ������� ��������� �� TCP/UDP.
	 � TCP ������������� ��������������� ���������� ���������� ��������. ������� ���������� ��������� �� ����.
	 ����� ���� ��������� ��������� TCP ����� ������� ��� �� �������������� ������� � ������������ ��� ��� ���� �����.

	 �� ������� ��������� ����������� "���������" TCP, � �� ������ ������� ������� � ��������������.
09.06.2013:
   ��� ��������� ��� ��� � ����������. ����� ����������� ���������������.
	 ������ ���: ��������� ���� � ��� �� ���� ��� TCP � UDP �����.
	 connect ��� Windows �������� � �����������.
	 WSA_XXX ����� ����������� ������� �� Socket-��.
	 �������� �������� ����� ������ � ������ � ����� �� ������ �������.
	 ������ ����� ������� ����. � ��������� ��������, � ������ � ������� ��� � �� ��������.

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    TODO:

28) ����� ����� � ������� "�����" - 
41) �������� ������������� �������� ����� �� ������� � ������� -
59) �������� ������� �������� ������ �� ����� ������� ������� ��� ���������� � ����� �� ����� ��������:
������ ����� � ����� � ������� ������ � ������� Bounding Box, � ����� �������������� � ������� (�� 3 �������),
��������: ������� ��� �������� ������ ���� ���-�� BB > 1 � ������������ � BB(�������� �������� �������� �����
 � �������). � ��� ������ Unity 3d - ����� ����.
67) ����������� ������ ���������: Tree, Terrain � �.�. -
68) ������ ����� � ������� -
76) ������� �������� id-path
77) path Effect �������� id_model � �������� ���������. MakerEffectDX - 
78) ��������� ����������� � ������ ����������� ����������� ��� ������� �� ������� ������ � ����������� �� ���������� �� ������
79) ��������� ����������� �� ������� � ������ �������� �� ������� �� ���� ������� ������, � ����������� �� ���������� � ����������� ����������� ��������.
82) �������� ���������� ���������� � BigJack - 
88) ��������: 1. ���������� �� ���������� �� ������ �� �������, 2. ������
3. ������ �������� (�������)
89) BSP ����� (������� ������-������ ������� ����������)
91) �������� �����
94) ������-��������� ���������, slave, master, superserver, client

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    INFO
���-�� ������� � �������:

0 - �������� �������� (������)
1 - ��������� Main (������)
2 - ClientMain Disconnect (������)
3 - Loader Map and Objects (�����)

���-�� ������� � �������:

0 - Qt (��� ������� - ������)
1 - ��������� Main (������)
2 - ServerTank Disconnect (������)
3 - ������� ����� ������� (������)
4 - Loader Map and Objects (�����) - ������ � ������ ��� 


������ ����������:
1) ManagerDirectX, �������� �� ��������� � ���� - 
2) ManagerModels, �������� �� �������� ��������� � �������, ������������ ������ - 
3) ManagerObjectDX, �������� �� ������� ����� +
4) 


  �� ������� �������:
ManagerDirectX ����� ������� �� ��������� ��������� ���� �� ��������� �����, ���� ���� ������ ��������� � ��������� ����� �������� �����.
�� ��������� ���������� ��� ������ �� ��������� ����� �� ManagerObjectDX.
�� ��������� �������� ������ ������ ������� ����� �� ������ � ���������� ������� �������������� �����(���� � ����������� �������� ...).

  �� ������� �������:


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    BUG:
1) ������ ��� �������� � ��� �� ������� ������� �������� �����-����� �� ������ �� ���� � ��� � ������������ ����������.


ABOUT SOMETHING:
19.04.02012: 
 - ��������� �������. �������� ����� �� �������� ���������. � ������������ 23 ������ �� ������� 27 ������. ����, ������� ������ ������.


*/

#endif