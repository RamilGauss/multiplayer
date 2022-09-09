/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#ifndef TreeJointH
#define TreeJointH

#include <vector>
#include <d3dx10math.h>

/*
ѕо иерархии
 M(n) = M(n)*M(n-1)*..*M(0) - с самого низа на верх
 причем M(0) = M(root)
             Mroot                 - по именам a
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

class TTreeJoint
{
  bool flgNeedSetup; // отладка
  bool flgWasSort;   // отладка
public:
  TTreeJoint();
  ~TTreeJoint();
  //--------------------------------------------------
  // заготовка дл€ настройки дерева
  struct TChild
  {
    std::string   name;
    D3DXMATRIXA16 matrix;
  };
  struct TPart
  {
    std::string name;
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
    D3DXMATRIXA16 world;// матрица root, относительно него все строитс€
    std::string root;   // название корн€
    std::vector<TPart*> vectorPart;// части дл€ соединени€, одна из этих частей всегда root (по имени)
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
  // Ќачальна€ настройка
  // 1. Setup(..)
  // 2. SetOrderMatrixByName(..)
  // 3. GetMatrix(..)
  // ...
  // »зменение одной из матриц
  // N. ChangeMatrix(..)
  // N+1. GetMatrix(..)
  // ...
  // M. SetDefault()
  // M+1. GetMatrix(..)
  //----------------------
  void Setup(TLoadedJoint* pLoadedTree);// -

  int GetCountPart();// +
  void SetOrderMatrixByName(std::vector<std::string>* order);// вызвать до вызова ChangeMatrix и GetMatrix // +
  
  // умножить матрицу по-умолчанию на новую матрицу и произвести изменени€ по всем дет€м
  void ChangeMatrix(std::string& name, D3DXMATRIXA16* matrix);// +
  // заполнить матрицей
  void GetMatrix(std::vector<D3DXMATRIXA16*>* matrix);// +
  // сбросить все матрицы в дефолт
  void SetDefault(); // +

protected:
  
  void Done();
protected:

  struct TNodeJoint
  {
    TNodeJoint* pParent;
    std::vector<TNodeJoint*> mVectorChild;
    std::string   name;
    D3DXMATRIXA16 matrixDef; // то что считали с файла настроек
    D3DXMATRIXA16 matrix;    // эта матрица получаетс€ умножением матрицы по-умолчанию на заданную матрицу через метод ChangeMatrix
    D3DXMATRIXA16 matrix_pro;// произведение по иерархии
    
    TNodeJoint():matrixDef(1.0f,0.0f,0.0f,0.0f,
                           0.0f,1.0f,0.0f,0.0f,
                           0.0f,0.0f,1.0f,0.0f,
                           0.0f,0.0f,0.0f,1.0f)
    {
      pParent = NULL;
      SetMatrixDef();
    }
    void SetMatrixDef(){matrix = matrixDef;}
  };
  
  TNodeJoint* mRoot;// корень есть всегда

  std::vector<TNodeJoint*> mVectorNode;// общее кол-во

  TLoadedJoint* mLoadedTree;// временно при Setup-е
  //------------------------------------------------------
  void ProductAllMatrix();
  void ProductChild(TNodeJoint* pNode);
  void ProductUp(TNodeJoint* pNode);


  void FindBranch(TPart* pPart, TNodeJoint* pParent);
};

#endif
