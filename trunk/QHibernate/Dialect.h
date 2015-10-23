
#ifndef QHibernate_Dialect
#define QHibernate_Dialect

#include <QHibernate/SqlTypes.h>
#include <QtCore/QSet>

class QString;

namespace QHibernate
{
	class Dialect
	{

	protected:

		void registerColumnType(SqlTypes sqlType, const QString& name);

		void registerColumnType(SqlTypes sqlType, int capacity, const QString& name);

	private:

		QSet<QString> mSqlKeywords;
	};

}

#endif