
#include <QHibernate/Select.h>

namespace QHibernate
{
	QString Select::getStatementString() const
	{
		QString statementString;

		statementString.append("select ");
		if (mSelectClause.isEmpty())
		{
			statementString.append("*");
		}
		else
		{
			statementString.append(mSelectClause);
		}

		statementString.append(" from ");
		statementString.append(mFromClause);

		if (!mOuterJoinsAfterFrom.isEmpty())
		{
			statementString.append(mOuterJoinsAfterFrom);
		}

		if (!mWhereClause.isEmpty() || 
			!mOuterJoinsAfterWhere.isEmpty())
		{
			statementString.append(" where ");

			if (!mOuterJoinsAfterWhere.isEmpty())
			{
				statementString.append(mOuterJoinsAfterWhere);
				if (!mWhereClause.isEmpty())
				{
					statementString.append(" and ");
				}
			}

			if (!mWhereClause.isEmpty())
			{
				statementString.append(mWhereClause);
			}
		}

		return statementString;
	}

	void Select::setSelectClause(const QString& selectClause)
	{
		mSelectClause = selectClause;
	}

	void Select::setFromClause(const QString& fromClause)
	{
		mFromClause = fromClause;
	}

	void Select::setWhereClause(const QString& whereClause)
	{
		mWhereClause = whereClause;
	}

	void Select::setOrderByClause(const QString& orderByClause)
	{
		mOrderByClause = orderByClause;
	}

	void Select::setOuterJoinsAfterFrom(const QString& outerJoinsAfterFrom)
	{
		mOuterJoinsAfterFrom = outerJoinsAfterFrom;
	}

	void Select::setOuterJoinsAfterWhere(const QString& outerJoinsAfterWhere)
	{
		mOuterJoinsAfterWhere = outerJoinsAfterWhere;
	}

	Select::Select()
	{

	}
}
