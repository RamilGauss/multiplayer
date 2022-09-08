#ifndef ApplicationProtocolDefH
#define ApplicationProtocolDefH

// типы пакетов
//-----------------------------------------------------------------------------
// REQUEST  - R

#define APPL_TYPE_BASE_REQUEST             0
#define APPL_TYPE_R_TRY_CONNECT_TO_SERVER  (APPL_TYPE_BASE_REQUEST+0) // попытка соединиться 
#define APPL_TYPE_R_FIRE                   (APPL_TYPE_BASE_REQUEST+1) // событие выстрела, на него отошлется время на перезарядку
#define APPL_TYPE_R_IN_FIGHT               (APPL_TYPE_BASE_REQUEST+2) // запрос на вступление в бой

#define APPL_TYPE_R_GET_LIST_TANK          (APPL_TYPE_BASE_REQUEST+3) // дать список танков

#define APPL_TYPE_R_EXIT_WAIT              (APPL_TYPE_BASE_REQUEST+4) // запрос выйти из ожидания боя

#define APPL_TYPE_R_CORRECT_PACKET         (APPL_TYPE_BASE_REQUEST+5) // запрос на корректирующий пакет

#define APPL_TYPE_R_EXIT_FIGHT             (APPL_TYPE_BASE_REQUEST+6) // запрос выйти из боя
//-----------------------------------------------------------------------------
// COMMAND - C

#define APPL_TYPE_BASE_COMMAND            	100

#define APPL_TYPE_C_KEY_EVENT             	(APPL_TYPE_BASE_COMMAND+0)

#define APPL_TYPE_С_CHOOSE_TANK             (APPL_TYPE_BASE_COMMAND+1) // выбор танка из гаража

// reserved
#define APPL_TYPE_C_DISCONNECT_FROM_SERVER  (APPL_TYPE_BASE_COMMAND+2) // отсоединиться 

//-----------------------------------------------------------------------------
// ANSWER - A

#define APPL_TYPE_BASE_ANSWER             200
#define APPL_TYPE_A_TRY_CONNECT_TO_SERVER (APPL_TYPE_BASE_ANSWER+0) // попытка соединиться 
#define APPL_TYPE_A_ECHO                  (APPL_TYPE_BASE_ANSWER+1) // эхо,пустышка
#define APPL_TYPE_A_FIRE_RELOAD           (APPL_TYPE_BASE_ANSWER+2) // в ответ на выстрел, время перезарядки
#define APPL_TYPE_A_IN_FIGHT              (APPL_TYPE_BASE_ANSWER+3) // ответ на запрос войти в бой
#define APPL_TYPE_A_GET_LIST_TANK         (APPL_TYPE_BASE_ANSWER+4) // дать список танков

#define APPL_TYPE_A_EXIT_WAIT             (APPL_TYPE_BASE_ANSWER+5) // ответ на желание выйти из очереди на ожидание
#define APPL_TYPE_A_EXIT_FIGHT            (APPL_TYPE_BASE_ANSWER+6) // ответ на желание выйти из боя

#define APPL_TYPE_A_END_FIGHT             (APPL_TYPE_BASE_ANSWER+7) // конец битвы, пока не знаю что передавать (результат боя, КЭП)

// в общем виде события боя, может быть несколько событий в одном пакете:
#define APPL_TYPE_A_CORRECT_PACKET_STATE_OBJECT  (APPL_TYPE_BASE_ANSWER+8)  // корректирующий пакет, не требует эффектов DX
#define APPL_TYPE_A_CORRECT_PACKET_STATE_TANK    (APPL_TYPE_BASE_ANSWER+9)  // корректирующий пакет, не требует эффектов DX

#define APPL_TYPE_A_SCORE                        (APPL_TYPE_BASE_ANSWER+10)  // и корректирующий пакет и просто пакет
// как ответ на пустой вопрос - вопроса не было
#define APPL_TYPE_A_EVENT_IN_FIGHT               (APPL_TYPE_BASE_ANSWER+11) // событие, показать на экране

//-----------------------------------------------------------------------------
// STREAM - S
// информация о танках, снарядах

#define APPL_TYPE_BASE_STREAM           300
#define APPL_TYPE_S_GARAGE              (APPL_TYPE_BASE_STREAM+0) // статистика: клиентов в бою и общее кол-во
#define APPL_TYPE_S_WAIT                (APPL_TYPE_BASE_STREAM+1) // статистика: кол-во ожидающих в очереди, в бою и общее кол-во
// размер пакета будет уточнятся
#define APPL_TYPE_S_LOAD_MAP            (APPL_TYPE_BASE_STREAM+2)// во время загрузки карты на сервере
#define APPL_TYPE_S_COUNT_DOWN          (APPL_TYPE_BASE_STREAM+3) // статистика обратного отсчета в начале боя
#define APPL_TYPE_S_FIGHT_COORD_BULLET  (APPL_TYPE_BASE_STREAM+4) // ориентация и скорости танков, снарядов

#define APPL_TYPE_S_ORIENT_AIM          (APPL_TYPE_BASE_STREAM+5) 
// приходит с клиента 10 раз в секунду, инфо по ориентации прицела, существует 3 вида: серверный, клиентский, мышиный текущий
// клиентский и серверный догоняют мышиный, а при нажатии правой кнопки мыши(фиксация) серверный начинает догонять клиентский.
// то есть с клиента приходит либо клиентский или текущий мышиный, но серверу все равно (та ориентация, к которой стремится серверный прицел (вращается башня или дуло))
//-----------------------------------------------------------------------------
#endif