
#include <QHibernate/SessionFactory.h>
#include <QHibernate/SessionFactoryData.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/QHibernate.h>

namespace QHibernate
{
	SessionFactory::SessionFactory()
		: mData(0)
	{
	}

	SessionFactory::SessionFactory(const Configuration& configuration)
		: mData(0)
	{
		setData(new SessionFactoryData());
		mData->setConfiguration(configuration);
	}

	SessionFactory::SessionFactory(const SessionFactory& sessionFactory)
		: mData(0)
	{
		*this = sessionFactory;
	}

	SessionFactory::~SessionFactory()
	{
		setData(0);
	}

	SessionFactory& SessionFactory::operator=( const SessionFactory& sessionFactory )
	{
		setData(sessionFactory.mData);
		return *this;
	}

	void SessionFactory::close()
	{
	}

	Session SessionFactory::openSession()
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		Session session(*this);
		try
		{
			session.open();
		}
		catch(HibernateException& exception)
		{		
			throw exception;
		}
		return session;
	}

	void SessionFactory::setDriverName( const QString& driverName )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		mData->setDriverName(driverName);
	}

	QString SessionFactory::getDriverName() const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		return mData->getDriverName();
	}

	void SessionFactory::setDatabaseName( const QString& databaseName )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		mData->setDatabaseName(databaseName);
	}

	QString SessionFactory::getDatabaseName() const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		return mData->getDatabaseName();
	}

	void SessionFactory::setHostName( const QString& hostName )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		mData->setHostName(hostName);
	}

	QString SessionFactory::getHostName() const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		return mData->getHostName();
	}

	void SessionFactory::setUserName( const QString& userName )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		mData->setUserName(userName);
	}

	QString SessionFactory::getUserName() const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		return mData->getUserName();
	}


	void SessionFactory::setPassword( const QString& password )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		mData->setPassword(password);
	}

	QString SessionFactory::getPassword() const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}
		return mData->getPassword();
	}

	const Class& SessionFactory::getClassMapping(const QString& className) const
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		ClassMap& classMappings =  mData->getClassMappings();
		if (!classMappings.contains(className))
		{
			throw HibernateException(QString("Class mapping does not exist for '%1'").arg(className));
		}

		return classMappings[className];
	}

	const Class& SessionFactory::getClassMapping(const Pointer<QObject>& qobject) const
	{
		return getClassMapping(qobject->metaObject()->className());
	}

	void SessionFactory::addClassMapping( const Class& classMapping )
	{
		if (isNull())
		{
			throw HibernateException("Session factory is null.");
		}

		ClassMap& classMappings = mData->getClassMappings();
		classMappings.insert(classMapping.getClassName(), classMapping);
	}

	void SessionFactory::setData(SessionFactoryData* data)
	{
		if (0 != mData)
		{
			mData->removeReference();
			SessionFactoryData::destroy(mData);
			mData = 0;
		}

		mData = data;

		if (0 != mData)
		{
			mData->addReference();
		}
	}

	bool SessionFactory::isValid() const
	{
		return (0 != mData);
	}

	bool SessionFactory::hasClassMapping(const QString& className) const
	{
		bool result = false;

		if (isValid())
		{
			result = mData->getClassMappings().contains(className);
		}

		return result;
	}

	bool SessionFactory::isNull() const
	{
		return (0 == mData);
	}
}
