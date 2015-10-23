
#ifndef QHibernate_ColumnMapping
#define QHibernate_ColumnMapping

#include <QtCore/QString>

namespace QHibernate
{
	class ColumnMapping
	{
	public:

		ColumnMapping();

		void setName(const QString& name);

		QString getName() const;

		void setLength(qint32 length);

		qint32 getLength() const;

	private:

		QString mName;

		qint32 mLength;
	};

}

#endif
