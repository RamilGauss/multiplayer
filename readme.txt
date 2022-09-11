 Start project.

Проект пока готов на 80% от того, что бы мне хотелось видеть в нем.
Сетевой транспорт на 75% готов. Нет физики, нет звука, нет БД. Но есть
GUI, графика и ядро.

Game Engine Tornado, MMO, for example World of Tanks clone 

MMOG

    English 
    
Requires:
    Boost 1.54, 
    Qt 4.5, 
    DirectX SDK June 2010
  See for more inaformation /doc/DeveloperManual
    
Directory with high registry first letter contains source code;
All other directories contains data (models, xml files etc.) and documentations.

BigJack - graphic engine,
Common, MyGUIEngine, Platforms - source code MyGUI,
DeveloperToolShare - common code for Developer,
DXUT - DXUT source code (subsystem of Big Jack, events),
Game - tornado.exe source code,
GameLib - core of game engine, main loop of events,
GUI - wrapper under MyGUI, used by Client
Melissa - MMO framework,
NetTransport - transport is used in Melissa,
QtLib - wrapper under Qt, used by Servers
Robert - wrapper under Bullet Engine,
Share - base library, common code,
Test - test modules,
Tool - disigner MyGUI and etc.,

doc - documentation and manual how use it.

//------------------------------------------------------------------------------------------
    Russian
    
Требуется:
    Boost 1.54, 
    Qt 4.5, 
    DirectX SDK June 2010
  
  Более подробно в /doc/DeveloperManual
    
Папка с Верхним регистром первой буквы содержит исходный код.
Все остальные папки содержат либо данные(модели, xml и т.д.), либо документацию.

BigJack - графический движок,
Common, MyGUIEngine, Platforms - исходный код MyGUI,
DeveloperToolShare - общий код для Разработчика,
DXUT - исходный код DXUT (подсистема BigJack, события),
Game - исполняемый код движка,
GameLib - ядро движка, главный поток, обработка событий,
GUI - надстройка над MyGUI, используется в графическом движке Клиента,
Melissa - MMO framework,
NetTransport - сетевой транспорт, используется Мелиссой,
QtLib - надстройка над Qt, используется в Сервере,
Robert - надстройка над Bullet Engine,
Share - базовые бибилиотеки, общий код,
Test - для тестирования модулей,
Tool - инструментарий, например дизайнер для MyGUI,

doc - документация, мануал как работать с движком.
