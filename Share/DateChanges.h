/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
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
������ �� �������� (IMakerObjectCommon). ��� ������, ������� ������� ��������� � �����, �����������
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
  - ������ ���������. �������� � ��� ������������ ������ ������ �� UDP 1,5�.
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
  �������� ��� 100 ���� 10 �� �������� � 150-250 ������� � ����� ������� (��� NetDoser ��� 
  ���������� ����� �������).
  - ���� ��� ������: ��� ������� ��������� ������ ������ �� ����� ������������� �������.
  ���� ������, �� ������ ����� � ���������� � ������� ���� ���, ��� �� �� ����������� ������ � ������.
  ���� ������ �� ������ ���������.
03.06.2013:
  ���������� ��������� �����
07.06.2013:
  ����� ������� � ������� ����������� ������� � ������ ��� ����� ������� ��������� �� TCP/UDP.
  � TCP ������������� �������������� ���������� ���������� ��������. ������� ���������� ��������� �� ����.
  ����� ���� ��������� ��������� TCP ����� ������� ��� �� �������������� ������� � ������������ ��� ��� ���� �����.

  �� ������� ��������� ����������� "���������" TCP, � �� ������ ������� ������� � ��� �����������.
09.06.2013:
  ��� ��������� ��� ��� � ����������. ����� ����������� ���������������.
  ������ ���: ��������� ���� � ��� �� ���� ��� TCP � UDP �����.
  connect ��� Windows �������� � �����������.
  WSA_XXX ����� ����������� ������� �� Socket-��.
  �������� �������� ����� ������ � ������ � ����� �� ������ �������.
  ������ ����� ������� ����. � ��������� ��������, � ������ � ������� ��� � �� ��������.
14.06.2013:
  ����� ���� ����� ��������� ���� � ��� �� ���� ��� listen � ��� connect. 
  ���� ������������ ���� reuse ��� ������.
16.06.2013:
  ������� ��������� �����. �� �� �������� � ��� �������.
  ���� � ��� ���, ��� ����� ������� ������ �� ����� ��� �������� ������ �� TCP
  ������ ������������ ������� ��������. ����� �������� � ������������� �� localhost ��� � WSA.
  ���� ��� �������. �������� �������� �� 17 ����. � ������ Melissa (���).
21.06.2013:
  �������� ������. ��� ���� � ��� ��� ���� ���� ����������� ���������� ��������� ����� �� ������������� ��������
  �������� send. ������ �� ����������, ��� �� ���� ����������.
  ���������� � ������ Melissa. �� ������� ���� ������� ������ ���������� ���������. ��� � �� ����.
  �� ��� ����� ���� �� 2-3 ������. ��� ���� ������ ����������� ������������ � ������ �����������.
  ��� ������ �������� ����� �����, ����� ����� ������ ������������ Melissa.
28.06.2013:
  ����� �������� �������, ������ Slave ����������. �������-�� ���� ����.
  ����� �����, ����� �������, ����� ������ ���. ���� ����� ��� ����� ��������� ����������
  ����.
02.07.2013:
  � �� ��������. ���� � ���, ��� � �������� ������������ ������ ����� ������������ ���� ����������
  �������. � ������� ���� ������� ��� ��. MyGUI � ��� ���������. � ��� ������� ������� ���.
  ���� ��� �������, ���� ������ � �����������.
  ����� ������� ��� ��� �� ���� �����������, ��� ��� �������� ������� � ������� ������.
  �� ����������� ������ �������. �� ������ ������ ��� ����� ������������� � ������ ����� ������������ ��������.
  ����� ������� ������ �������� ������, Melissa � �������� ������.
  1. �������� ��������� �������� (�����, ����� �����) � ������ ���������� Slave, Master, SuperServer.
  2. �������� Melissa. ��� ��� �������� � �������� ���������� ����������. ��� ������
  ��� ������� ��������� ����, ��� ��� ������.
  ������� Newton � Bullet(���� ����� ������ ����������!). ��� ����������, ���� �� ��� ���
  ������� ODE (��� ��� � ������ ��� �� �����������).
03.07.2013:
  �������� TNetTransport_UDP. ������� TArrayObject �� std::map.
  ��� ���� ������ � ��������. �������� � ��������� �� ����������� �� GBaseLib.
  ���� ����� ����� ����� ������������ ���, ��� ��� TCP.
  ����� ������ ����� ������� ��������� ��������, ������� ���� � 23-00 �� 6-20. 
  ������ ����� ����������, ��� ����� :). ����� �������, ����� ����� ���������.
04.07.2013:
  ���� ����������� ���������� �������� ������� �������.
  ����� ����� �� ���������� ����������� ������� �������.
  ���� ������ ������� �������, ���� ����� �������������� ������� �� ���������� 
  (������������ �� � Work), ���� ����� �������� ������������ ������� � ��������� � TSrcEvent.
  �� ������ �������� ����� Work ��������� ���� �������, ��������, ����
  ����� ����� ������ �� ������� �� ������.
05.07.2013:
  Peace of Tanks ? ��� ������ (��������)
  Piece of Tanks ? ��� ������ (��������)
  �� �������� Peace � Piece - �������� ���� � �� �� ���,
  �� ��� �������� ��� � ��� � �����
07.07.2013:
  ������������ Peace of Tanks.
  ���� � �������� ����� ���� ������ ����������� ������,
  ���� 2+2=4, ����� ��������� ������ ������ � �.�.
  ��������������� ���� ������� �������, �� ������ �����������.
  �� ���� ���������� ��� ���� std::map, �� �� ��� Boost-�.
  RakNet ������ ��������. Cloud �� 100$ � �����.
08.07.2013:
  �������� ������ �����. ������ ���� ����� ����� ����� ����� ����������
  � ��������� � TBase.
11.07.2013:
  ��� ���� ����� ������������ BulletPhysics.
  1. ��� Bullet ����� ������������. � Newton-� ��� �����, ���� ��� �� ������� ������.
  �� �� ����� ��� �������� ����� ��������� � ������ ��������� �����������, �� � ������������ �� ���
  ��������, ����������� ��� ���� � ������ �� �������, � ������������ ���������, �� ��� ���������.
  � ������ ������� �������� ���� � ����������� ��� ����������� �����������, � � Newton ��� �����������
  ��� ���������, �� ��� �����.
  2. Bullet ������������ � GTA 5, � ��� ��� ���-�� �� ������. ����� �������� ������ ��� ������������
  ����������� � ����� � ������� �� �����.
  3. ������ ������ ��������� �� Bullet, �������� ����������� - ������ ��� �����, �� ������� ��������.
18.07.2013:
  ����� ��������� ������, � ������ ����� - �������� �� ��� ������� WSAWaitForMultipleEvents �������
  ����� 64. ��� Windows XP ��� �����. ���� ���������� ��� ��� Windows 7 � ��� Linux (poll).
  ��� ����� - ���� ����� ������� ������� �� 64 ������, ���� ����� RegisterWaitForSingleObject.
19.07.2013:
  ��! � ��������� ��� ��������� �� MS. ���� ����� ������� WSAPoll, �� ��� �������� � Windows Vista.
  ������ poll, ���� �������� ������� ����� ������� �����������. ���� ����� ����������
  ����� (���� �� 500), �� ����� ����� ��� Windows XP ������������ ������������� �����, � ���
  Windows 7 WSAPoll.
22.07.2013:
  �� ��� WSAPoll. � ������� ������� � ������� ����� ������� �� ������.
  MainThread ������� hEvent, ������ ������� ������ �������� � ������� SetEvent
  � ��������� ������.
24.07.2013:
  Boost ���������� Completion Port ��� Windows (����� ������ �����). 
  ���� ������ ��������� �� WSAWaitEvent,
  ������ ������� ���������, ���������� �� Boost(�� ������ ��������� �������). ��������� Boost 1.54.
  �.�. ����� 3 ���� ���������� (Boost, TCP_UDP(Win32) � UDP(Win32/Linux)).
25.07.2013:
  � ������ boost ����� ��������� ���������� �� glib � GBaseLib. �������� ���-�� ����������,
  �� � �� �� ����� ����� ������� ������� �� boost.
06.08.2013:
  � ������� boost asio ��� Windows XP ������������ ���-�� ���������� - 156, 
  ��� Windows 7 - 500 (� ��� �� ������).
  ������� � async_connect �� connect, ������ ����� 220, �� ����� ���� �� � ����, ����
  ������ ���� ���� �� ������ ����.
  ������� ��������� CPU Intel Core2Duo E6400. �� �����(�� 100%) ���� 100 UDP 1000 �� 1 ����� �� 1350 ����,
  ���� Intel Core2Duo E8400 � Core2Duo E7400 ����� ���� 220, ���� �� ���������� (2-5%).
  � �� ������� �� �� ���� � ����������, �� �� � ��, �� �� � ������ ������.
  ���� �� � ���� Win7, 4 �� � ��� ����� ��������. � ����� ������ ������ ��� �� ���-�� �� ��������.

  ����� ������� ��� ��������� �����. ������ ������� ������� ����� �� ������� � �������� �� 
  ���� � E7400, ����������� ��������� ��� � �����. ������� ��� �� ���� ������ 
  20 ������� �� 1350 ���� � ��������� 100 �� �� 500 �������� (��� ������������ 10000 ��������
  �� 1 ����� �� 100 ��). ��� ��� ������ � 1350 ���� ������� �����, ������ ����� �������� 100-200 ����(�� ���� 
  ����� � 10 ���, � ��� ������ 100 000 ��������). "�������" - ������ �������� 10-20% �� ������ ����. 
  ��� �� ���������� ����� �� ������ ������. �� ���� ��� 4 �������� ���������� ���� �������� 5% �� ����� ��������.
  ��� ������� ������������ CPU x8 Xeon X5550HT 2666 MHz (���� ��������� ������).

  ���, ���, ���. ������ ������ � 10-00 ����, 3 �����, ����� � 16-00, ���� ���� ���������, ���� ���������� ��������.
07.08.2013:
  CPU x8 Xeon X5550HT 2666 MHz ����� 1600$, �� ��� ��� ����������������� ������. �����
  Intel Core2Duo i7 (1000$ � 6 ������) ������, 32 �� ������.
14.08.2013:
  BOOST_FOREACH ������ �������� � map � set, ���� ����� ������ ���������� ������.
	�������� ������: ������������ BOOST_FOREACH(TMapClass::value_type& bit, mMap)
	                                bit.second->Func();
27.08.2013:
  ���� Slave � ������ �������� �����.
  ������ ���� ����� ������� ����������� �������.
29.08.2013:
  ��� ��������� ����� ���������� �������� ������ ��������. ������ Self-To-Self, �� ����
  ���� ����� �������� �������� ������ ���� �� ������, �.�. �������� �������� �� ����� � ����
  ������, � ����� ���� ������ �������� ������������ �����.
  �������� �������� ��������, ���������.!!!!!!
30.08.2013:
  ��� �� ����� ����������� �� ���������:
    1. ������� ���� ���� �������� �������� ��� ����� ����. �� ���� ������, ���������
  ���������� �� ����� � ����� �������. ������ ���������� �������, ������� ������
  ������ �������� ������������ �����. ����� �������� ������ ��� ������ � �������.
  �� ���� ��������� ���� �������� ������� ��������� TControlScenario
    2. ����� �������� ���������. �.�. ����� ������� ���� ������ � ��������, �� �������� ������ ��������,
  ���������������� � ������ �����������, ����������. �������� ���� �������� ��������� �������� - IContextScenario.
  �.�. �������� ���������� ������, �� ���� �������� ������� ���������. � �������� ��� ������, � ��������
  �������� ��������. ��� ������ ��������� � ������(���� ����� ���� ��������� � ���� ������).
    3. ����� ��������� �������� ��� ��������� ������� �� ��������? ��������, �������������� �������.
  �������� �������������� (�������� �������� - RCM). �������� �� ����� - ��� C1. �������� ������ �����������
  � ������, ������ ������������ ��� ���� RCM. ������� Begin, ��������� ������� �� ��.
  ��� ��������� ����������? �������� ��� ��, �������� ��� ��. ���� ������ ����������
  ����� ������ ������ ���������� � ��� �� ����������. 
  �������: bool Begin(IContextScenario* pCSc)
05.09.2013:
  ���-��� � ���������. �������� - ������, �������� �������� - ������. ��������
  ��� ������ ���� ������ ���������, ������.
  ��� ���������� �� �������� ����� ������ ������� (������� ����� ����������� ������� ����������
  ������ ��������). ���� �� ����� ���� ������� ���������� � ������� � ������� ���������.
  �� ��� �� ���� ������ ���� � ����� ���������, ������ ������� ���� Context()->.

23.09.2013:
  �������� �� ������ 18 ��������, ���� ������ ������ �_�.
  ���� �������� �������� ��� ������������ ����������� ������ ������. � ������ ��� ���������
  ����� �����. ������� ������ ����������� �������, ������� ������������ �� ����� -c.
  ����� ���� ������� ���������� GUI. GUI ����� ����� ��������� ��������������� ����� DevTool, 
  �� ������ ������� ����� �� ����� ����� �� �������� � �������.
  � ����� ���� ������� ������� ����� �����������, � ������ ��� �������� ����� ���� �������.
24.09.2013:
  ������ ����. �������� GUI � ������. ���������� ������������ ����. ��� ������������ 
  ���� ����� ������ � IGame.
  ����, fps ��� ������� �� 10! ������ � �� ����!
  ��� ����� ������ ����������� �� ����.
26.09.2013:
	������� ������ QtLib, ������ ��� ������� ����� ������������ GUI.
	������ 3 ������� ������������ 5, �������� GetServerDeveloperTool
	�� GetSlaveDeveloperTool, GetMasterDeveloperTool, GetSuperServerDeveloperTool.
	������ ������.
27.09.2013:
	��� ������ �� ��� QtLib. ������ ����� ������ ������� �� ������ Qt ��� �������.
	������ ����� ����� ���-�� �������� � �����, ������ ������� � ��������� �������
	� ���������� ��������� � ������ Qt � �����-�� �� �������.
03.10.2013:
  �������� ������ "������"!!!
07.10.2013:
  ������� ������. �������� ���� � ������ �������. ����� Refresh �� �������������.
  �����, ��������� LoginClient. ����� ������� ��������, ��� ������� �����
  ��������������, �� ��� ���������� 90% ���� ������.
09.10.2013:
  �������� � ������������ �������� LoginClient. 
  ScenarioLoginClient->ColdMaster->HotMaster->Accept ��� Reject
  Accept -> Queue ��� Accept (� ����������� �� �������� ��������)
  -> Context->ScenarioLoginClient
  ������ �� Github.com :). �������, ����-�� ������ ����������. �� ����� ��� ������ �� ����������,
  �� ������� ���� ���������.
10.10.2013:
  ����������� ScenarioLoginClient, � ������ ���������� � ������� �������� �������,
  ���� ��� ����� �� ��������.
26.10.2013:
  ���� ��������� � ScenarioLoginClient 4 �����: Client, Slave, Master � SuperServer,
	������ �� ������� �������� �� ��������� �������, ���������������, ��������������,
	������. ���� ������� �����, �� �������� ����������� ��� ������, � ��� ������ ���� ����������
	��� ������, � ������� ����� ������ ���� � ScenarioLoginClient, � ��� ���������� ����
	char where, �� ���� ���� ������������ �����, �.�. ScenarioLoginClient ����������
	�� ���� ����� �� ������ ������ �����.
	����� ������ ��������� ����� ���� ������ ���� 4 ����� (C,S,M,SS).
	������ ������� ����� ����� ����� ���� ��������� ������������� ���� � ��������� ��������� ������ ScenarioLoginClient.
30.10.2013:
  ������������� ��������� � �������. ������� ���� ������� � lib �� dll, ������� �����
  � ������������� Melissa �� MMOEngine � �.�.
07.11.2013:
  ������� ����� ������ ���������. ����������� �����.
08.11.2013:
  LoginClient �����! �������� ����������� �������� ������ ����� �� ������ ������ ���������� ��������.
	����� �������� �������� ����������� �������. � ��������� ������ �������������� ������� � ���, ������ ��� ��������!
	� ���, ���� �������������� � ��������� ����������� �������.
12.11.2013:
  ������� �������� DisconnectClient ��� ������� � Slave.
  �������� LoginClient ������� � ������ ���������� (�������� ��� ������ ��� ������������ �� ����� ������ ��������).
  �������� �������������� � �������� � ���������.
14.11.2013:
  ������ ��������� ���.
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
89) BSP ����� (������� ������-������ ������� ����������)
91) �������� �����
94) ������-��������� ���������, slave, master, superserver, client
96) ���� ����� ��� ������� ������ � �������� � ��� ��� - ��������� ������� � ������������ ������.
97) ������������� ���� ��� Windows XP � Boost: QueryPerformanceCounter ���������� false.
��� �������� ��������� ����� ���������� ����� �� ������ ������� this_thread::sleep_for().

98) � ������� std::list<std::list<id_session> > mListGroupWaitRecommutation - ��������� ���� � ������
99) ���������� ��� ���������� �� *.lib � *.dll.
100) � DeveloperTool ������ �������, ������������ TLogger* (???).
101) ������������� BigJack->GraphicEngine, Robert->PhysicEngine, Melissa->MMOEngine, 
������ 99 �������.
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    INFO

  �� ������� �������:

  �� ������� �������:


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    BUG:

*/

#endif