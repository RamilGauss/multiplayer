/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CallBackRegistratorH
#define CallBackRegistratorH

#include <boost/signals2/signal.hpp>
//-------------------------------------------------------------
template < typename T1=void,
           typename T2=void,
           typename T3=void,
           typename T4=void,
           typename T5=void,
           typename T6=void> 
class TCallBackRegistratorBase
{
protected:
  boost::signals2::signal<void (T1)> sig;
public:
  // например Register(&TClass::Method, &object)
  template<typename F, class A1>
  void Register(F pFunc, A1 pObject)
  {
    sig.connect(boost::bind(pFunc, pObject, _1));
  }
  // например Unregister(&object)
  template<class A1>
  void Unregister(A1 pObject)
  {
    sig.disconnect(pObject);
  }
};
//-------------------------------------------------------------
template<typename T1> 
class TCallBackRegistrator1 : public TCallBackRegistratorBase<T1>
{
public: void Notify(T1 t1){sig(t1);}
};
//-------------------------------------------------------------
template<typename T1,typename T2> 
class TCallBackRegistrator2 : public TCallBackRegistratorBase<T1,T2>
{
public: void Notify(T1 t1,T2 t2){sig(t1,t2);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3> 
class TCallBackRegistrator3 : public TCallBackRegistratorBase<T1,T2,T3>
{
public: void Notify(T1 t1,T2 t2,T3 t3){sig(t1,t2,t3);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4> 
class TCallBackRegistrator4 : public TCallBackRegistratorBase<T1,T2,T3,T4>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4){sig(t1,t2,t3,t4);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4,typename T5> 
class TCallBackRegistrator5 : public TCallBackRegistratorBase<T1,T2,T3,T4,T5>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5){sig(t1,t2,t3,t4,t5);}
};
//-------------------------------------------------------------
template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> 
class TCallBackRegistrator6 : public TCallBackRegistratorBase<T1,T2,T3,T4,T5,T6>
{
public: void Notify(T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6){sig(t1,t2,t3,t4,t5,t6);}
};
//-------------------------------------------------------------


#endif
