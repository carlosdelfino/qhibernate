
#ifndef QHibernate_CustomUserType
#define QHibernate_CustomUserType

#include <QHibernate/QHibernate_global.h>

class QVariant;

namespace QHibernate
{
	class QHIBERNATE_API CustomUserType
	{
	public:

		virtual ~CustomUserType();

		virtual void setPropertyValue(QVariant& component, 
			qint32 propertyIndex, 
			const QVariant& propertyValue ) const = 0;

		virtual QVariant getPropertyValue(const QVariant& component,
			qint32 propertyIndex) const = 0; 
	};
}

#endif

