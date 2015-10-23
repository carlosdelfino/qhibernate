
#ifndef QHibernate_Insert
#define QHibernate_Insert

#include <QtCore/QHash>
#include <QtCore/QString>

namespace QHibernate
{
	class Insert
	{
	public:

		void addColumn(const QString& name, const QString& value);	

		QString getStatementString() const;

		void setTableName(const QString& tableName);

	private:

		QHash<QString, QString> mColumns;

		QString mTableName;
	};
}


#endif
