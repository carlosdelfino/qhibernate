
#ifndef QHibernate_Relationship
#define QHibernate_Relationship

#include <QHibernate\PropertyMapping.h>

namespace QHibernate
{
	class Relationship
		: public PropertyMapping
	{
	public:

		void setClassName(const QString& className);

		const QString& getClassName() const;

	private:

		QString mClassName;
	};
}


#endif
