#ifndef IImageNotifyH
#define IImageNotifyH


#include "UIDobject.h"
//-----------------------------------------------------------------------------
// ������ ���������, ������������ ��������, ������������ ImageFile
#define MFL_COUNT       10

#define MFL_OPEN        0   // ���� ������
#define MFL_CLOSE       1   // ���� ������
#define MFL_RELOAD      2   // ���� ��� ���������� �� ��
#define MFL_INSERT      3   // �������� ������
#define MFL_CHANGE      4   // ������� ������
#define MFL_DELETE      6   // ������ ������

#if MFL_DELETE > MFL_COUNT
  #error MFL_DELETE > MFL_COUNT
#endif
//-----------------------------------------------------------------------------

// ����� �������, ������������ ���� - ��������� ������
#define IFM_OPEN        (1<<0)  // �������� ��������� MFL_OPEN
#define IFM_CLOSE       (1<<1)  // �������� ��������� MFL_CLOSE
#define IFM_INSERT      (1<<2)  // �������� ��������� MFL_INSERT
#define IFM_DELETE      (1<<3)  // �������� ��������� MFL_DELETE
#define IFM_CHANGE      (1<<6)  // �������� ��������� MFL_CHANGE
//.....................
#define IFM_ALL (IFM_OPEN | IFM_CLOSE | IFM_INSERT | IFM_DELETE | IFM_CHANGE)

//=============================================================================
//=============================================================================
class TImageFile;

class IImageNotify
{
protected:
public:
  // ���������� ������� �������� �����
  virtual void OnOpen( TImageFile* ) {}

  // ���������� ������� �������� �����
  virtual void OnClose( TImageFile* ) {}

  // ���������� ������� �������� �������
  // index - �������
  // p     - ��������� �� ������
  virtual void OnDelete( TImageFile*, int , PUIDobject ) {}

  // ���������� ������� �������� ������� ��������
  virtual void OnDeleteArray( TImageFile* ) {}

  // ���������� ������� ��������� �������
  // oldIndex - ������ ������ � �������
  // newIndex - ����� ������ � �������
  // p     - ��������� �� ������
  virtual void OnChange( TImageFile* , int , int , PUIDobject  ) {}

  // ���������� ������� ���������� �������
  // index - �������
  // p     - ��������� �� ������
  virtual void OnInsert( TImageFile* , int , PUIDobject  ) {}
};
//=============================================================================
//=============================================================================

#endif
