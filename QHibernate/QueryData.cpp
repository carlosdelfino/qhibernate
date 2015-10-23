
#include <QHibernate/QueryData.h>

namespace QHibernate
{
	QueryData::QueryData(const Session& session)
		: mSession(session)
	{
	}

	Session QueryData::getSession() const
	{
		return mSession;
	}

	void QueryData::setQueryString(const QString& queryString)
	{
		mQueryString = queryString;
	}

	const QString& QueryData::getQueryString() const
	{
		return mQueryString;
	}

}

