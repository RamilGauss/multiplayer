/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ClientMain.h"

#include "Precompiled.h"
#include <atlconv.h>
#include "../GameLib/IClientDeveloperTool.h"
#include <boost/asio/ip/impl/address_v4.ipp>
#include "GlobalParam.h"

TClientMain::TClientMain()
{
  IsOpen = false;
}
//------------------------------------------------------
TClientMain::~TClientMain()
{

}
//-------------------------------------------------------------------------------------
void TClientMain::Activate()
{
  assignWidget(bEnter,"bEnter");
  assignWidget(bExit, "bExit");

  assignWidget(ebIP,    "ebIP");
  assignWidget(ebPort,  "ebPort");
  assignWidget(ebLogin, "ebLogin");

  bEnter->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Enter);
  bExit ->eventMouseButtonClick += MyGUI::newDelegate(this, &TClientMain::sl_Exit);
}
//-------------------------------------------------------------------------------------
void TClientMain::sl_Enter(MyGUI::Widget* _sender)
{
  USES_CONVERSION;
  std::string sPort = W2A((LPCWSTR)ebPort->getOnlyText().data());
  int port = atoi(sPort.data());

  std::string sIP = W2A((LPCWSTR)ebIP->getOnlyText().data());
  unsigned int ip = boost::asio::ip::address_v4::from_string(sIP.data()).to_ulong();

  std::string sLogin = W2A((LPCWSTR)ebLogin->getOnlyText().data());

  IClientDeveloperTool::TComponentClient* pComponent = 
    IClientDeveloperTool::Singleton()->GetComponent();
  // настройка сети
  if(IsOpen==false)
    IsOpen = pComponent->mNetClient->Open(port);

  BL_ASSERT(IsOpen);
  pComponent->mNetClient->Login( ip, MASTER_PORT, (void*)sLogin.data(), sLogin.length());
}
//-------------------------------------------------------------------------------------
void TClientMain::sl_Exit(MyGUI::Widget* _sender)
{
  IClientDeveloperTool::Singleton()->Exit();
  //Hide();
}
//-------------------------------------------------------------------------------------
const char* TClientMain::GetNameLayout()
{
  return "ClientMain.layout";
}
//-------------------------------------------------------------------------------------
void* TClientMain::GetParent()
{
  return nullptr;
}
//-------------------------------------------------------------------------------------
void TClientMain::SetupTabChild()
{
  mVectorChild_Tab.push_back(ebIP);
  mVectorChild_Tab.push_back(ebLogin);
  mVectorChild_Tab.push_back(ebPort);
}
//-------------------------------------------------------------------------------------
void TClientMain::KeyEvent(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)
{
  switch(_key.getValue())
  {
    case MyGUI::KeyCode::Return:
      sl_Enter(_sender);
      break;
    default:;
  }
}
//-------------------------------------------------------------------------------------
