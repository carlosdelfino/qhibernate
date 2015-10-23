
#ifndef QHibernate_Restrictions
#define QHibernate_Restrictions

#include <QHibernate\Criterion.h>
#include <QtCore\QString> 

class QString;
class QVariant;

namespace QHibernate
{
	class Restrictions
	{
	public:

		static Criterion eq(const QString& propertyName, const QVariant& value);
	};
}

#endif
