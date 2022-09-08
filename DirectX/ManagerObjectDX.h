#ifndef ManagerObjectDXH
#define ManagerObjectDXH


class TManagerObjectDX
{




public:
  TManagerObjectDX();
  ~TManagerObjectDX();

  // список объектов загруженной карты
  // статические, серверные(разрушаемые неподвижные, танки, снаряды), клиентские объекты

  void Clean();

protected:

};

extern TManagerObjectDX GlobalManagerObjectDX;

#endif