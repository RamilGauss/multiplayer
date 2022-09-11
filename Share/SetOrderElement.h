/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef SET_ORDER_ELEMENT_H
#define SET_ORDER_ELEMENT_H

#include "TypeDef.h"
#include <boost/bimap/bimap.hpp>
#include <vector>

/*
  ��������� ���������, ������ �� ������� ����� ���������� ���� ������,
  ��� ����������� ������� ��������� ���� �������.
  ���������� ������ ���������� � ������. 
  � �������� ����� ����������� �� �� ������ ����� �������.
*/

class SHARE_EI TSetOrderElement
{
	typedef boost::bimaps::bimap<unsigned int,unsigned int> bmUintUint;

  typedef std::vector<unsigned int> TVectorUint;

  bmUintUint   mMapKeyInnerIndex;
  TVectorUint  mVecSortInnerIndex; // �������������

  unsigned int mNextAddInnerID;

public:
  TSetOrderElement();
  ~TSetOrderElement();
  
  // ���������
  bool FindIndexByClientKey(unsigned int key, int& index);

  // ����������/��������
  void AddKeyAtEnd(unsigned int key);
  void DeleteKey(unsigned int key);
	bool DeleteFirst(unsigned int& key);

  void Clear();
private:
  bool FindByKey(unsigned int key);
  void ReserveForVector();
  void DeleteFromVectorByInnerIndex(unsigned int val);
	bool FindKeyByInnerIndex(unsigned int inner_index, unsigned int& key);
};

#endif

