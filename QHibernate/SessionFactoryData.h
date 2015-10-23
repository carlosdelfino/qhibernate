
#ifndef QHibernate_SessionFactoryData
#define QHibernate_SessionFactoryData

#include <QHibernate\ReferenceCounter.h>
#include <QHibernate\Configuration.h>
#include <QHibernate\ClassMap.h>

namespace QHibernate
{
	class SessionFactoryData
		: public ReferenceCounter<SessionFactoryData>
	{
	public:

		void setConfiguration(const Configuration& configuration);

		void setDriverName(const QString& driverName);

		const QString& getDriverName() const;

		void setDatabaseName(const QString& databaseName);

		const QString& getDatabaseName() const;

		void setHostName(const QString& hostName);

		const QString& getHostName() const;

		void setUserName(const QString& userName);

		const QString& getUserName() const;

		void setPassword(const QString& password);

		const QString& getPassword() const;

		ClassMap& getClassMappings();

	private:

		Configuration mConfiguration;

		ClassMap mClassMappings;

		QString mDriverName;

		QString mDatabaseName;

		QString mHostName;

		QString mUserName;

		QString mPassword;
	};
}

#endif