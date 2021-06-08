#ifndef ZIP_FILE_GLOBAL_H
#define ZIP_FILE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZIP_FILE_LIBRARY)
#  define ZIP_FILE_EXPORT Q_DECL_EXPORT
#else
#  define ZIP_FILE_EXPORT
#endif

#endif // ZIP_FILE_GLOBAL_H
