#ifndef TransportProtocolPacketH
#define TransportProtocolPacketH


// квитанции K о получении высылаются если пришел REQUEST, ANSWER, COMMAND

// на STREAM квитанция не нужна (устаревшая информация может перекрыть свежую, а это хреново)
// игрок увидит старые позиции танков


#if defined( WIN32 )
#pragma pack(push, 1)
#endif

struct TIP_Port
{
  unsigned int ip;
  unsigned short port;
};
// базовые структуры
//-----------------------------------------------------------------------------
struct TPrefixTransport
{
  unsigned char  type;   // K - квитанция, S - нет гарантии доставки, P - гарантия доставки, С-обмен циклическими номерами, нужен для синхронизации транспорта
  unsigned short cn_in;     // циклический номер для определения свежести при Rcv
  unsigned short cn_out;    // циклический номер при Send
  unsigned char  cntTry;
	guint32 time_ms;// для проверки необходимости отсылки неподтвержденного пакета
  TIP_Port ip_port_dst;  //
	TIP_Port ip_port_src;  //
  TPrefixTransport()
  {
    cntTry = 0;
  };
};
//-----------------------------------------------------------------------------


#if defined( WIN32 )
#pragma pack(pop)
#endif

#endif