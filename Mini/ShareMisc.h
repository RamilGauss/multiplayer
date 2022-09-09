#ifndef ShareMiscH
#define ShareMiscH

#include <stddef.h>

#define CHECK_RET(f) \
if(f==false) \
return false;


#ifdef WIN32
  #define _PACK
#else
  #define _PACK __attribute__ ((packed))
#endif


namespace nsShareMisc
{
	struct t_time
	{
		unsigned long sec;
		unsigned long ns;
		unsigned long delta_ns;
	} _PACK;
	//--------------------------------------------------------------------
	struct t_time_s
	{
		unsigned long sec;
		unsigned long ns;
	} _PACK;
	//--------------------------------------------------------------------
};


#endif
