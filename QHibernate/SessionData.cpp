
#include <QHibernate/SessionData.h>

namespace QHibernate
{
	SessionData::SessionData(const SessionFactory& sessionFactory)
		: mSessionFactory(sessionFactory)
	{
	}

	void SessionData::setSessionFactory( const SessionFactory& sessionFactory )
	{
		mSessionFactory = sessionFactory;
	}

	void SessionData::setDatabase( const QSqlDatabase& sqlDatabase )
	{
		mQSqlDatabase = sqlDatabase;
	}

	QSqlDatabase& SessionData::getDatabase()
	{
		return mQSqlDatabase;
	}

	const SessionFactory& SessionData::getSessionFactory() const
	{
		return mSessionFactory;
	}

	SessionData::~SessionData()
	{
		if (mQSqlDatabase.isOpen())
		{
			mQSqlDatabase.close();
		}
	}
}
