
#include <QHibernate\SessionFactoryData.h>

namespace QHibernate
{
	void SessionFactoryData::setConfiguration(const Configuration& configuration)
	{
		mConfiguration = configuration;
	}

	void SessionFactoryData::setDriverName(const QString& driverName)
	{
		mDriverName = driverName;
	}

	const QString& SessionFactoryData::getDriverName() const
	{
		return mDriverName;
	}

	void SessionFactoryData::setDatabaseName(const QString& databaseName)
	{
		mDatabaseName = databaseName;
	}

	const QString& SessionFactoryData::getDatabaseName() const
	{
		return mDatabaseName;
	}

	void SessionFactoryData::setHostName(const QString& hostName)
	{
		mHostName = hostName;
	}

	const QString& SessionFactoryData::getHostName() const
	{
		return mHostName;
	}

	void SessionFactoryData::setUserName(const QString& userName)
	{
		mUserName = userName;
	}

	const QString& SessionFactoryData::getUserName() const
	{
		return mUserName;
	}

	void SessionFactoryData::setPassword(const QString& password)
	{
		mPassword = password;
	}

	const QString& SessionFactoryData::getPassword() const
	{
		return mPassword;
	}

	ClassMap& SessionFactoryData::getClassMappings()
	{
		return mClassMappings;
	}

}

