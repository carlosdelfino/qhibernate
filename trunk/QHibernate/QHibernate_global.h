#ifndef QHibernate_GLOBAL_H
#define QHibernate_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QHIBERNATE_LIB
# define QHIBERNATE_API Q_DECL_EXPORT
#else
# define QHIBERNATE_API Q_DECL_IMPORT
#endif


#endif // QHibernate_GLOBAL_H
