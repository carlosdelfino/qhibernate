
#include <QHibernate\Relationship.h>

namespace QHibernate
{
	void Relationship::setClassName(const QString& className)
	{
		mClassName = className;
	}

	const QString& Relationship::getClassName() const
	{
		return mClassName;
	}
}
