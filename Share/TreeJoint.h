/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef TreeJointH
#define TreeJointH

#include <vector>
#include <map>
#include <string>
#include "Struct3D.h"
#include "TypeDef.h"


/*
�� ��������
 M(n) = M(n)*M(n-1)*..*M(0) - � ������ ���� �� ����
 ������ M(0) = M(root)
             Mroot                 - �� ������ a
          /   |   \  
         M1   M1   M1              -  b c d
         |         |  \          
         M2        M2  M2          -  e f g
                   | 
                   M3              -   h
       Mh = Mh*Mf*Md*Ma;
       World = Mh*world;
       WorldViewProj = World*View*Proj;
*/


class SHARE_EI TTreeJoint
{
  bool flgNeedSetup; // �������
  bool flgWasSort;   // �������
public:
  TTreeJoint();
  ~TTreeJoint();
  //--------------------------------------------------
  // ��������� ��� ��������� ������
  struct TChild
  {
    std::string   name;
    nsStruct3D::TMatrix16 matrix;
  };
  struct TPart
  {
    std::string name;
    int numUse;// ����� ����� ��������
    std::vector<TChild*> vectorChild;
    ~TPart()
    {
      int cnt = vectorChild.size();
      for(int i = 0 ; i < cnt ; i++)
        delete vectorChild[i];
    }
    TPart& operator =(const TPart& p)
    {
      name = p.name;
      numUse = p.numUse;
      int cnt = p.vectorChild.size();
      for(int i = 0 ; i < cnt ; i++)
      {
        TChild* pChild = new TChild;
        *pChild = *p.vectorChild[i];
        vectorChild.push_back(pChild);
      }
      return *this;
    }
  };
  struct TLoadedJoint
  {
    // ������� root, ������������ ���� ��� ��������
    nsStruct3D::TMatrix16 world;
    std::string root;   // �������� �����
    std::vector<TPart*> vectorPart;// ����� ��� ����������, ���� �� ���� ������ ������ root (�� �����)
    ~TLoadedJoint()
    {
      int cnt = vectorPart.size();
      for(int i = 0 ; i < cnt ; i++)
        delete vectorPart[i];
    }
    TLoadedJoint& operator =(const TLoadedJoint& l)
    {
      world = l.world;
      root  = l.root;
      int cnt = l.vectorPart.size();
      for(int i = 0 ; i < cnt ; i++)
      {
        TPart* pPart = new TPart;
        *pPart = *l.vectorPart[i];
        vectorPart.push_back(pPart);
      }
      return *this;
    }
  };
  //--------------------------------------------------
  // INTERFACE
  // ��������� ���������
  // 1. Setup(..)
  // 2. SetOrderMatrixByName(..)
  // 3. GetMatrix(..)
  // ...
  // ��������� ����� �� ������
  // N. ChangeMatrix(..)
  // N+1. GetMatrix(..)
  // ...
  // M. SetDefault()
  // M+1. GetMatrix(..)
  //----------------------
  typedef std::map<std::string, int> TNumUseMap;

  void Setup(TLoadedJoint* pLoadedTree,TNumUseMap* mapUse);// +

  int GetCountPart();// +
  void SetOrderMatrixByName(std::vector<std::string>* order);// ������� �� ������ ChangeMatrix � GetMatrix // +
  
  // �������� ������� ��-��������� �� ����� ������� � ���������� ��������� �� ���� �����
  void ChangeMatrix(std::string& name, nsStruct3D::TMatrix16* matrix, bool def = true);// +
  // ��������� ��������
  void GetMatrix(std::vector<nsStruct3D::TMatrix16*>* matrix);// +
  // �������� ��� ������� � ������
  void SetDefault(); // +

protected:
  
  void Done();
protected:

  struct TNodeJoint
  {
    TNodeJoint* pParent;
    std::vector<TNodeJoint*> mVectorChild;
    std::string   name;
    nsStruct3D::TMatrix16 matrixDef; // �� ��� ������� � ����� ��������
    nsStruct3D::TMatrix16 matrix;    // ��� ������� ���������� ���������� ������� ��-��������� �� �������� ������� ����� ����� ChangeMatrix
    nsStruct3D::TMatrix16 matrix_pro;// ������������ �� ��������
    
    TNodeJoint()
    {
      SetMatrixIdentity(&matrixDef);
      pParent = NULL;
      SetMatrixDef();
    }
    void SetMatrixDef(){matrix = matrixDef;}
  };
  
  TNodeJoint* mRoot;// ������ ���� ������

  std::vector<TNodeJoint*> mVectorNode;// ����� ���-��

  TLoadedJoint* mLoadedTree;// �������� ��� Setup-�
  TNumUseMap* mMapUse;
  //------------------------------------------------------
  void ProductAllMatrix();
  void ProductChild(TNodeJoint* pNode);
  void ProductUp(TNodeJoint* pNode);


  void FindBranch(TPart* pPart, TNodeJoint* pParent);
};

#endif
