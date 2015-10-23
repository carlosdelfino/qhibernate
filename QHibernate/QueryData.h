
#ifndef QHibernate_QueryData
#define QHibernate_QueryData

#include <QHibernate/Session.h>
#include <QHibernate/ReferenceCounter.h>

namespace QHibernate
{
	class QueryData
		: public ReferenceCounter<QueryData>
	{
	public:

		QueryData(const Session& session);

		Session getSession() const;

		void setQueryString(const QString& queryString);

		const QString& getQueryString() const;

	private:

		Session mSession;

		QString mQueryString;
	};
}

#endif
