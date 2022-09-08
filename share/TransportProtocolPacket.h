#ifndef TransportProtocolPacketH
#define TransportProtocolPacketH


// ��������� K � ��������� ���������� ���� ������ REQUEST, ANSWER, COMMAND

// �� STREAM ��������� �� ����� (���������� ���������� ����� ��������� ������, � ��� �������)
// ����� ������ ������ ������� ������


#if defined( WIN32 )
#pragma pack(push, 1)
#endif

struct TIP_Port
{
  unsigned int ip;
  unsigned short port;
};
// ������� ���������
//-----------------------------------------------------------------------------
struct TPrefixTransport
{
  unsigned char  type;   // K - ���������, S - ��� �������� ��������, P - �������� ��������, �-����� ������������ ��������, ����� ��� ������������� ����������
  unsigned short cn_in;     // ����������� ����� ��� ����������� �������� ��� Rcv
  unsigned short cn_out;    // ����������� ����� ��� Send
  unsigned char  cntTry;
	guint32 time_ms;// ��� �������� ������������� ������� ����������������� ������
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