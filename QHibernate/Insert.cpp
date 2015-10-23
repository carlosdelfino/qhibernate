
#include <QHibernate/Insert.h>

namespace QHibernate
{
	void Insert::addColumn(const QString& name, 
		const QString& value)
	{
		mColumns.insert(name, value);
	}

	QString Insert::getStatementString() const
	{
		QString statement;

		statement.append("insert into ");
		statement.append(mTableName);

		if(mColumns.isEmpty())
		{
			statement.append(" ( ) ");
		}
		else
		{
			QString columnsPart;
			QString valuesPart;

			QHash<QString, QString>::const_iterator itor = mColumns.constBegin();
			for (; itor != mColumns.end(); ++itor)
			{
				if (!columnsPart.isEmpty())
				{
					columnsPart.append(" ,");
					valuesPart.append(" ,");
				}

				columnsPart.append(itor.key());
				valuesPart.append(itor.value());


			}

			statement.append(" (");
			statement.append(columnsPart);
			statement.append(") values (");
			statement.append(valuesPart);
			statement.append(")");
		}
		return statement;
	}

	void Insert::setTableName( const QString& tableName )
	{
		mTableName = tableName;
	}
}

