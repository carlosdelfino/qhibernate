
#include <QHibernate/ObjectProxy.h>
#include <QHibernate/QtSqlHelper.h>
#include <QHibernate/QObjectPersister.h>
#include <QHibernate/HibernateException.h>

#include <QtSql/QSqlRecord>

namespace QHibernate
{
	ObjectProxy::ObjectProxy(const Session& session, 
		const ManyToOne& manyToOne, 
		const QVariant& filterValue)
		: mSession(session)
		, mManyToOne(manyToOne)
		, mFilterValue(filterValue)
	{
	}

	ObjectProxy::ObjectProxy(const Session& session, 
		const OneToOne& oneToOne, 
		const QVariant& filterValue )
		: mSession(session)
		, mOneToOne(oneToOne)
		, mFilterValue(filterValue)
	{
	}

	ObjectProxy::~ObjectProxy()
	{

	}

	void* ObjectProxy::getPointer()
	{
		QString className;
		if (mManyToOne.isValid())
		{
			className = mManyToOne.getClassName();
		}
		else if (mOneToOne.isValid())
		{
			className = mOneToOne.getClassName();
		}
		else
		{
			throw HibernateException("Invalid state for Object Proxy.");
		}

		QObject* objectPointer = 0;

		try
		{
			QSqlRecord sqlRecord = QtSqlHelper::getRecord(mSession, className, mFilterValue);
			QObjectPersister persister(className, mSession);
			persister.deserialize(sqlRecord, objectPointer);
		}
		catch (HibernateException& e)
		{

		}

		return objectPointer;
	}
}
