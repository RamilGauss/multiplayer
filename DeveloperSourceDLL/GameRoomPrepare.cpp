#include "GameRoomPrepare.h"

#include "Precompiled.h"
#include <atlconv.h>


TGameRoomPrepare::TGameRoomPrepare()
{

}
//------------------------------------------------------
TGameRoomPrepare::~TGameRoomPrepare()
{

}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::Activate()
{
  //assignWidget(bEnter,"bEnter");
  //assignWidget(bExit, "bExit");

  //assignWidget(ebIP,    "ebIP");
  //assignWidget(ebPort,  "ebPort");
  //assignWidget(ebLogin, "ebLogin");

  //bEnter->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Enter);
  //bExit ->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Exit);
}
//-------------------------------------------------------------------------------------
//void TClientMain::sl_Enter(MyGUI::Widget* _sender)
//{
//
//}
////-------------------------------------------------------------------------------------
//void TClientMain::sl_Exit(MyGUI::Widget* _sender)
//{
//  Close();
//}
//-------------------------------------------------------------------------------------
//void TClientMain::sl_IP(MyGUI::EditBox* _sender)
//{
//  USES_CONVERSION;
//  std::string sA = W2A((LPCWSTR)_sender->getOnlyText().data());
//  int a = 0;
//}
//-------------------------------------------------------------------------------------
const char* TGameRoomPrepare::GetNameLayout()
{
  return "GameRoomPrepare.layout";
}
//-------------------------------------------------------------------------------------
void* TGameRoomPrepare::GetParent()
{
  return nullptr;
}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::SetupTabChild()
{
  //mVectorChild_Tab.push_back(ebIP);
  //mVectorChild_Tab.push_back(ebLogin);
  //mVectorChild_Tab.push_back(ebPort);
}
//-------------------------------------------------------------------------------------
void TGameRoomPrepare::KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  switch(_key.getValue())
  {
    case MyGUI::KeyCode::Return:
      //sl_Enter(_sender);
      break;
    default:;
  }
}
//-------------------------------------------------------------------------------------
