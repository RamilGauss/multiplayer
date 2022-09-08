#ifndef QBASELIB_GLOBAL_H
#define QBASELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QBASELIB_LIB
# define QBASELIB_EXPORT Q_DECL_EXPORT
#else
# define QBASELIB_EXPORT Q_DECL_IMPORT
#endif

#endif // QBASELIB_GLOBAL_H
