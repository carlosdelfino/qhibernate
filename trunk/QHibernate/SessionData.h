
#ifndef QHibernate_SessionData
#define QHibernate_SessionData

#include <QHibernate/ReferenceCounter.h>
#include <QHibernate/SessionFactory.h>
#include <QtSql/QSqlDatabase>

namespace QHibernate
{
	class SessionData
		: public ReferenceCounter<SessionData>
	{
	public:

		SessionData(const SessionFactory& sessionFactory);

		~SessionData();

		void setSessionFactory(const SessionFactory& sessionFactory);

		const SessionFactory& getSessionFactory() const;

		void setDatabase(const QSqlDatabase& sqlDatabase);

		QSqlDatabase& getDatabase();

	private:

		SessionFactory mSessionFactory;

		QSqlDatabase mQSqlDatabase;
	};

}

#endif
