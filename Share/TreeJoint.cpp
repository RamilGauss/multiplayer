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

#include "TreeJoint.h"
#include "BL_Debug.h"
using namespace std;

TTreeJoint::TTreeJoint()
{
  mLoadedTree = NULL;
  flgNeedSetup = true;
  flgWasSort   = false;
  mRoot = NULL;
}
//-------------------------------------------------------------------------
TTreeJoint::~TTreeJoint()
{
  Done();
}
//-------------------------------------------------------------------------
void TTreeJoint::SetOrderMatrixByName(vector<string>* order)// ������� �� ������ ChangeMatrix � GetMatrix
{
  // ����������� ������� � ������� ���� � ������������ � ��������
  int cnt = order->size();
  BL_ASSERT(cnt==GetCountPart());
  for(int i = 0 ; i < cnt ; i++)
  {
    bool flgFound = false;
    for(int j = i ; j < cnt ; j++)
    {    
      TNodeJoint* pNode_j = mVectorNode.at(j);
      if(order->operator [](i).compare(pNode_j->name)==0)
      {
        flgFound = true;
        if(i==j)
          break;// � ������ �����
        // ������ �������
        TNodeJoint* pNode_i = mVectorNode.at(i);
        mVectorNode.at(i) = pNode_j;
        mVectorNode.at(j) = pNode_i;
      }
    }
    BL_ASSERT(flgFound);
  }
}
//-------------------------------------------------------------------------
int TTreeJoint::GetCountPart()
{
  return mVectorNode.size();
}
//-------------------------------------------------------------------------
void TTreeJoint::Setup(TLoadedJoint* pLoadedTree)
{
  Done();
  mLoadedTree = pLoadedTree;
  //-----------------------------------------
  // ����� root
  int cnt = pLoadedTree->vectorPart.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    TPart* pPart = pLoadedTree->vectorPart.at(i);
    if(pPart->name.compare(pLoadedTree->root)==0)
    {
      // ����� root
      TNodeJoint* pNode = new TNodeJoint;
      mVectorNode.push_back(pNode);
      mRoot = pNode;
      //---------------------------
      mRoot->name = pLoadedTree->root;
      mRoot->matrixDef = pLoadedTree->world;
      mRoot->SetMatrixDef();
      // ������ ������ ����� ���� ������ ����� ��� ����� �����
      FindBranch(pPart, mRoot);// + ������������� ��������� TNodeJoint
      flgNeedSetup = false;
      break;
    }
  }
  mLoadedTree = NULL;
  BL_ASSERT(flgNeedSetup==false);
  //-----------------------------------------
  ProductAllMatrix();
}
//-------------------------------------------------------------------------
void TTreeJoint::ChangeMatrix(std::string& name, D3DXMATRIXA16* matrix)
{
  int cnt = GetCountPart();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TNodeJoint* pNode = mVectorNode.at(i);
    if(pNode->name.compare(name)==0)
    {
      pNode->matrix = *matrix;
      break;
    }
  }
  ProductAllMatrix();
}
//-------------------------------------------------------------------------
// ��������� ��������
void TTreeJoint::GetMatrix(std::vector<D3DXMATRIXA16*>* matrix)
{
  int cnt = GetCountPart();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TNodeJoint* pNode = mVectorNode.at(i);
    D3DXMATRIXA16* pM = matrix->operator [](i);
    *pM = pNode->matrix_pro;// ���������� �� ��������
  }
}
//-------------------------------------------------------------------------
// �������� ��� ������� � ������
void TTreeJoint::SetDefault()
{
  int cnt = GetCountPart();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TNodeJoint* pNode = mVectorNode.at(i);
    pNode->SetMatrixDef();
  }
  ProductAllMatrix();
}
//-------------------------------------------------------------------------
void TTreeJoint::Done()
{
  int cnt = GetCountPart();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TNodeJoint* pNode = mVectorNode.at(i);
    delete pNode;
  }
  mVectorNode.clear();
  mRoot = NULL;
}
//-------------------------------------------------------------------------
void TTreeJoint::ProductAllMatrix()
{
  BL_ASSERT(mRoot);
  //-----------------------------------------------------
  ProductChild(mRoot);
}
//-------------------------------------------------------------------------
void TTreeJoint::ProductChild(TNodeJoint* pNode)
{
  int cnt = pNode->mVectorChild.size();
  for(int i = 0 ; i < cnt ; i++)
  {
    TNodeJoint* pChild = pNode->mVectorChild.at(i);
    ProductUp(pChild);// �������� ������� ������� Node ����� � ����� � 
    //���� ����������� �����
    ProductChild(pChild);
  }
}
//-------------------------------------------------------------------------
void TTreeJoint::ProductUp(TNodeJoint* pNode)
{
  pNode->matrix_pro = pNode->matrix;
  TNodeJoint* pParent = pNode->pParent;
  while(pParent)
  {
    pNode->matrix_pro *= pParent->matrix;
    pParent = pParent->pParent;
  }
}
//-------------------------------------------------------------------------
void TTreeJoint::FindBranch(TPart* pPart, TNodeJoint* pParent)
{
  int cntTree = mLoadedTree->vectorPart.size();
  // ��������� ����� � ����� ����������� 
  int cnt = pPart->vectorChild.size();
  for(int i = 0; i < cnt ; i++)
  {
    TNodeJoint* pNode = new TNodeJoint;
    pNode->pParent = pParent;// �������� �������� � ��������
    pParent->mVectorChild.push_back(pNode);// �������� � ������ ����� ��������
    mVectorNode.push_back(pNode);// � ����� ������

    TChild* pChild = pPart->vectorChild.at(i);
    pNode->name = pChild->name;// ���
    pNode->matrixDef = pChild->matrix;// �������
    pNode->SetMatrixDef();
    // �����
    for(int i = 0 ; i < cnt ; i++)
    {
      TPart* pFPart = mLoadedTree->vectorPart.at(i);
      if(pFPart->name.compare(pNode->name)==0)
      {
        FindBranch(pFPart,pNode);
      }
    }
  }
}
//-------------------------------------------------------------------------
