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
 TBaseObject, TanksLib.lib, TManagerGUI, TBaseGUI_DX.
 - ����� ������� ��������� ������� OBJ. �� ���� EditorModel ���� �� �����.
07.08.2012:
 - ����� ����������� � Enterprise Architect.
08.08.2012:
 - ������ ������� � ������. � ����� ������������. (doc/����� �����������.EAP)
 - ������ v0.040
15.08.2012:
 - ����� �������� BigJack. �������� �������� TBaseObjectDX::SetModel - ���������� �������.
20.08.2012:
 - ������� TCallbackRegistrator, ������� std::set<...>.
29.08.2012:
 - ������� �� DXUT. ������ ����� Qt �������� � DXUT.
 - BigJack ���������. ���� ���� � ���, ��� Subset � DrawSubset ��� ����� 0, � ������ ���� ����� 1.
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    TODO:

28) ����� ����� � ������� "�����" - 
41) �������� ������������� �������� ����� �� ������� � ������� -
59) �������� ������� �������� ������ �� ����� ������� ������� ��� ���������� � ����� �� ����� ��������:
������ ����� � ����� � ������� ������ � ������� Bounding Box, � ����� �������������� � ������� (�� 3 �������),
��������: ������� ��� �������� ������� ���� ���-�� BB > 1 � ����������� � BB(�������� �������� �������� �����
 � �������). � ��� ������ Unity 3d - ����� ����.
65) TBaseObject-> TBaseObjectDX + TBaseObjectPrediction ->TBaseCommon->TTank+TBullet+TTree+TBush+TTerrain+TBuilding
������������ ����������� ������������.
66) ������� �������������� TManagerCommonObject + TGameForm = Translate+VisualEvent+KeyMouseEvent -
67) ����������� ������ ���������: Tree, Terrain � �.�. -
68) ������ ����� � ������� -
69) TBaseObjectDX::SetModel - 
70) ���������� EditorModel -
71) ������� TManagerObjectCommon ����������� � ��������� ������ � TManagerObjectCommonClient - 
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
    INFO
���-�� ������� � �������:(������ � ������� �� ����� ;) )

0 - Qt (������)
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