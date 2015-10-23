
#ifndef QHibernate_Delete
#define QHibernate_Delete

#include <QtCore\QString>
#include <QtCore\QHash>

namespace QHibernate
{
	class Delete
	{
	public:

		void setTableName(const QString& tableName);

		QString getTableName() const;

		QString getStatementString() const;

		void setWhereClause(const QString& whereClause);

		QString getWhereClause() const;

		void addPrimaryKeyColumn(const QString& columnName, 
			const QString& valueExpression);

	protected:

	private:

		QString mTableName;

		QString mWhereClause;

		QHash<QString, QString> mPrimaryKeyColumns;
	};

}


#endif
