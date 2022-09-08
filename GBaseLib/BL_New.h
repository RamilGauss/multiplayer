#ifndef BL_NewH
#define BL_NewH

#include <glib.h>


#if (defined(__WIN32__) || defined(_WIN32) || defined(WIN32)) && (defined(_DEBUG))

  #define NEW_DECLARE( class_name ) 

#else

  #define NEW_DECLARE( class_name ) \
    void* operator new ( size_t size )  { return g_slice_alloc( (gsize)size );  } \
    void  operator delete( void* p )    { g_slice_free( class_name, p ); }

#endif

#endif // BL_NewH
