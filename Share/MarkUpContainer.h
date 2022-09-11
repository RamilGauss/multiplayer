/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MarkUpContainerH
#define MarkUpContainerH

#include <map>
#include <string>
#include <vector>
#include "TypeDef.h"
#include "Container.h"

/*
    ������ ���������.
    �������� ������:
  1. �������� ���������� ���������.
  2. ������� ������ ������� ������ �� �����.
  3. ���������� ��������� �� ������ � ������� ��������� �� ������, ������� ���� ����������.
  
  ������:
  -------------
  �onst             
  -------------
  count = 2   - ���������
  var
  var
  -------------
*/

class SHARE_EI TMarkUpContainer
{
  typedef std::map<std::string,int> TMapStrInt;
  
  TMapStrInt mMarkUp;
  TContainer mC;
 
public:
//--------------------------------------------------------------------
  typedef enum
  {
    eConst,    // ���� ����������� �������
    eVar,      // ������ �� ������������� ��������, � ������ ���� ������� ���-�� ���������
    eMarkUp,   // ���� �� ����� �������� �������, ������� ����� �������� � ������� TMarkUpContainer
  } eType;
  struct TDescConst
  {
    int size;// ����
  };
  struct TDescVar
  {
    int sizeCnt; // ����
    int sizeVar; // ���� 
  };
  struct TDescMarkUp
  {
    int sizeSize;// ������ ���� "������", ����
  };
  struct TCommonDesc
  {
    eType type;
    std::string name; 
    union
    {
      TDescConst  c;
      TDescVar    v;
      TDescMarkUp m;
    };
  };
  //--------------------------------------------------------------------------------------
  TMarkUpContainer();
  virtual ~TMarkUpContainer();

  void SetMarkUp(std::vector<TCommonDesc>* pVecSize);// �������� ���������� +

  // ?? �������� ����� �������� �������� ��� ������ � ����������� (�� ����� � map �������� �� 3 �� 9 ��� ��� �������)
  // ���� ����������� ������� �� ���������
  // � ������ ���� ������� ����� ���� eVar, �� ������������ ��������� ��� ���������,
  // ����� ������������ ������ name, � ��������� ��������� ������������
  // counter - ����� � ������� ���������� ���-�� 
  // ������ ������������ ������ ���� type_counter==false
  void* Get(const char* name, bool type_counter = true, int index = 0); // +

  // ��������� - ���� ������� ������ ����������
  void* GetPtr() const;// +
  int   GetSize();// +
  
  // ������� ������� ��������� �� ������� ������ (����������� ������ ���� ������ ��������)
  // ���-�� ���� ����� ������� ������, ������� �������� ������ ������ � �� �������� ������� ����� ��������
  bool Set(void* p, int size);// +
  
  // ���������� ������
  void Done();// +
  
  // � ������ ���� ���������� ���-�� ��������� � ������� ���� eVar
  // �������� ������������ �������� ������ � �������� � �������
  void Update();// +

	// ��������� - ��� ������� ������ � �����. ������ ���� ������� ��� ������� �� ����� ������.
	void  SetDataAppendix(void* p, int size);
	
	void* GetDataAppendix(int& size);
	int GetSizeAppendix();
//--------------------------------------------------------------------
protected:
  struct TDesc_Private
  {
    TCommonDesc c;
    int shift;// �������� �� ������ ������, ����
    int cntVar; // ������������� ������ ���� ��� eVar
    int sizeMarkUp;
    TDesc_Private()
    {
      cntVar     = 0;
      sizeMarkUp = 0;
    }
  };

  typedef std::vector<TDesc_Private> TVectorDesc;
  TVectorDesc mVectorSection;

private:
  int GetValueBy(char* ptr, int sizeCnt);
  void SetCountBy(char* ptr, int sizeCnt, int v);

  // ���� ptr=NULL, �� ������������� ������ ����, 
  // � ������� ��� �� ������������������� ��������
  int GetSize(TDesc_Private& desc_p, char* ptr);
  void ZeroTensileField();// �������� ����, ���������� �� ������ � ���-�� ���������� ������ �����

  int GetSizeByDesc(TDesc_Private& desc_p);

	void SetSizeAppendix(int v);

	void* GetPtrAppendix();
protected:
	// ���������
	int mSizeAppendix;
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------

#endif

