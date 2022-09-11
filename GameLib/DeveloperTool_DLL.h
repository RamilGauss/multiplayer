/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef DeveloperTool_DLLH
#define DeveloperTool_DLLH

class IDeveloperTool;
class IClientDeveloperTool;
class IServerDeveloperTool;

#if defined(WIN32)
  #define DllExport  extern "C" __declspec( dllexport )
#else
  #define DllExport extern "C"
#endif

#define StrGetClientDeveloperTool "GetClientDeveloperTool"
#define StrGetServerDeveloperTool "GetServerDeveloperTool"
#define StrFreeDeveloperTool      "FreeDeveloperTool"

typedef IClientDeveloperTool* (*FuncGetClientDeveloperTool)(int);
typedef IServerDeveloperTool* (*FuncGetServerDeveloperTool)(int);
typedef void (*FuncFreeDeveloperTool)(IDeveloperTool*);

DllExport IClientDeveloperTool* GetClientDeveloperTool( int variant_use);
DllExport IServerDeveloperTool* GetServerDeveloperTool( int variant_use);

DllExport void FreeDeveloperTool(IDeveloperTool* p);


#endif
