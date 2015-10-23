
#include <QHibernate/Delete.h>

namespace QHibernate
{
	void Delete::setTableName(const QString& tableName)
	{
		mTableName = tableName;
	}

	QString Delete::getTableName() const
	{
		return mTableName;
	}

	QString Delete::getWhereClause() const
	{
		return mWhereClause;
	}

	void Delete::setWhereClause(const QString& whereClause)
	{
		mWhereClause = whereClause;
	}

	QString Delete::getStatementString() const
	{
		QString statement;
		statement.append("delete from ");
		statement.append(mTableName);

		if (!mWhereClause.isEmpty() || !mPrimaryKeyColumns.isEmpty())
		{
			statement.append(" where ");
		}

		QHash<QString, QString>::const_iterator itor 
			= mPrimaryKeyColumns.constBegin();

		for(; itor != mPrimaryKeyColumns.end(); ++itor)
		{
			if (itor != mPrimaryKeyColumns.constBegin())
			{
				statement.append(" and ");
			}

			statement.append(itor.key());
			statement.append(" = ");
			statement.append(itor.value());
		}

		if (!mWhereClause.isEmpty())
		{
			if (!mPrimaryKeyColumns.isEmpty())
			{
				statement.append(" and ");
			}

			statement.append(mWhereClause);
		}

		return statement;
	}

	void Delete::addPrimaryKeyColumn(const QString& columnName, 
		const QString& valueExpression)
	{
		mPrimaryKeyColumns.insert(columnName, valueExpression);
	}

}

