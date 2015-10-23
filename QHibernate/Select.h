
#ifndef QHibernate_Select
#define QHibernate_Select

#include <QtCore/QString>

namespace QHibernate
{
	class Select
	{
	public:

		Select();

		QString getStatementString() const;

		void setSelectClause(const QString& selectClause);

		void setFromClause(const QString& fromClause);

		void setWhereClause(const QString& whereClause);

		void setOrderByClause(const QString& orderByClause);

		void setOuterJoinsAfterFrom(const QString& outerJoinsAfterFrom);

		void setOuterJoinsAfterWhere(const QString& outerJoinsAfterWhere);

	private:

		QString mSelectClause;

		QString mFromClause;

		QString mWhereClause;

		QString mOrderByClause;

		QString mGroupByClause;

		QString mOuterJoinsAfterFrom;

		QString mOuterJoinsAfterWhere;

		//const Dialect& mDialect;
	};
}


#endif
