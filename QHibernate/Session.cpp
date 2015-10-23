
#include <QHibernate/Session.h>
#include <QHibernate/Transaction.h>
#include <QHibernate/Class.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/Query.h>
#include <QHibernate/QObjectPersister.h>
#include <QHibernate/SessionData.h>
#include <QHibernate/QtSqlHelper.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlIndex>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

namespace QHibernate
{
	Session::Session()
		: mData(0)
	{
	}

	Session::Session(const SessionFactory& sessionFactory)
		: mData(0)
	{
		QString driverName = sessionFactory.getDriverName();
		static int i = 0;
		QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase(driverName, "connection_" + QString::number(i++));

		if (!sqlDatabase.isValid())
		{
			QSqlError sqlError = sqlDatabase.lastError();
			throw HibernateException(sqlError.text());
		}

		QString hostName = sessionFactory.getHostName();
		sqlDatabase.setHostName(hostName);

		QString databaseName = sessionFactory.getDatabaseName();
		sqlDatabase.setDatabaseName(databaseName);

		QString userName = sessionFactory.getUserName();
		sqlDatabase.setUserName(userName);

		QString password = sessionFactory.getPassword();
		sqlDatabase.setPassword(password);

		setData(new SessionData(sessionFactory));
		mData->setSessionFactory(sessionFactory);

		mData->setDatabase(sqlDatabase);
	}

	Session::Session(const Session& session)
		: mData(0)
	{
		*this = session;
	}


	Session::~Session()
	{
		setData(0);
	}

	Session& Session::operator=(const Session& session)
	{
		setData(session.mData);
		return *this;
	}

	void Session::open()
	{
		if (!isNull())
		{
			QSqlDatabase& sqlDatabase = mData->getDatabase();
			bool connected = sqlDatabase.open();

			if (!connected) 
			{
				QSqlError error = sqlDatabase.lastError();
				QString message = error.text();

				throw HibernateException(message);
			}
		}
		else
		{
			throw HibernateException("Could not open null session.");
		}
	}

	void Session::close()
	{
		if (!isNull())
		{
			QSqlDatabase& sqlDatabase = mData->getDatabase();
			sqlDatabase.close();
		}
		else
		{
			throw HibernateException("Could not close null session.");
		}
	}

	void Session::beginTransaction()
	{
	}

	Transaction Session::getTransaction() const
	{
		return Transaction(*this);
	} 

	Query Session::createQuery(const QString& queryString)
	{
		if (isNull())
		{
			throw HibernateException("Could not create query. Session is null.");		
		}

		return Query(*this, queryString);
	}

	const SessionFactory& Session::getSessionFactory() const
	{
		if (isNull())
		{
			throw HibernateException("Could not get session factory from null session.");
		}

		return mData->getSessionFactory();
	}

	const QSqlDatabase& Session::getConnection() const
	{
		if (isNull())
		{
			throw HibernateException("Could not get QSqlConnection from null session.");
		}

		return mData->getDatabase();
	}

	StrongPointer<QObject> Session::get(const QMetaObject& metaObject, 
		const QVariant& idValue)
	{
		if (isNull())
		{
			throw HibernateException("Could not get object from null session.");
		}

		QString className = metaObject.className();
		QSqlRecord sqlRecord = QtSqlHelper::getRecord(*this, 
			className, 
			idValue);

		StrongPointer<QObject> qobject;

		if (!sqlRecord.isEmpty())
		{
			QObjectPersister persister(className, *this);
			qobject = persister.deserialize(sqlRecord);
		}

		return qobject;
	}

	void Session::erase(StrongPointer<QObject> object)
	{
		if (isNull())
		{
			throw HibernateException("Could not erase from null session.");
		}

		if (object.isNull())
		{
			throw HibernateException("Could not erase null object.");
		}	

		const QMetaObject* metaObject = object->metaObject();
		QString className = metaObject->className();
		Class classMapping = getClassMapping(className);

		if (classMapping.isSubclass())
		{
			QString parentClass = classMapping.getExtends();
			const QMetaObject* superMetaObject = metaObject->superClass();
			deleteInternal(*superMetaObject, object);
		}
		else
		{
			deleteInternal(*metaObject, object);
		}
	}

	void Session::update(StrongPointer<QObject> qobject)
	{
		if (isNull())
		{
			throw HibernateException("Could not update from null session.");	
		}

		if (qobject.isNull())
		{
			throw HibernateException("Could not update null object.");
		}

		const SessionFactory& sessionFactory = mData->getSessionFactory();
		const Class& classMapping = sessionFactory.getClassMapping(qobject);

		const QMetaObject* metaObject = qobject->metaObject();

		if (classMapping.isSubclass())
		{		
			const QMetaObject* superMetaObject = metaObject->superClass();
			updateInternal(*superMetaObject, qobject);
		}

		updateInternal(*metaObject, qobject);
	}

	QSharedPointer<QSqlTableModel> Session::createSqlTableModel(const Pointer<QObject>& qobject)
	{
		if (qobject.isNull())
		{
			throw HibernateException("Could not create QSqlTableModel for null object.");
		}

		return createSqlTableModel(*(qobject->metaObject()));
	}

	QSharedPointer<QSqlTableModel> Session::createSqlTableModel(const QMetaObject& metaObject)
	{
		QSqlDatabase& sqlDatabase = mData->getDatabase();
		QSharedPointer<QSqlTableModel> sqlTableModel 
			= QSharedPointer<QSqlTableModel>(new QSqlTableModel(0, sqlDatabase));	

		const SessionFactory& sessionFactory = mData->getSessionFactory();
		const Class& classMapping 
			= sessionFactory.getClassMapping(metaObject.className());


		sqlTableModel->setTable(classMapping.getTableName());
		checkSqlTableModelError(sqlTableModel);

		sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

		return sqlTableModel;
	}

	void Session::checkSqlTableModelError( QSharedPointer<QSqlTableModel> sqlTableModel )
	{
		QSqlError error = sqlTableModel->lastError();
		if (error.isValid())
		{
			throw HibernateException(error.text());
		}
	}

	void Session::save(const StrongPointer<QObject> qobject)
	{
		if (isNull())
		{
			throw HibernateException("Could not save from null session.");
		}

		if (qobject.isNull())
		{
			throw HibernateException("Could not save from null object.");
		}

		const SessionFactory& sessionFactory = mData->getSessionFactory();
		const QMetaObject* metaObject = qobject->metaObject();

		Class classMapping = sessionFactory.getClassMapping(qobject);
		if (classMapping.isSubclass())
		{
			const QMetaObject* superMetaObject = metaObject->superClass();
			saveInternal(*superMetaObject, qobject);
		}	

		saveInternal(*metaObject, qobject);

	}

	QVariant Session::getIdValue(StrongPointer<QObject> qobject) const
	{
		const SessionFactory& sessionFactory = mData->getSessionFactory();
		Class classMapping = sessionFactory.getClassMapping(qobject);

		if (classMapping.isSubclass())
		{
			const QMetaObject* metaObject = qobject->metaObject();
			classMapping = sessionFactory.getClassMapping(metaObject->superClass()->className());
		}

		IdentifierMapping idMapping = classMapping.getIdentifierMapping();
		QByteArray idName = idMapping.getName().toLocal8Bit();

		QVariant idValue = qobject->property(idName);
		return idValue;
	}

	void Session::saveOrUpdate(StrongPointer<QObject> qobject)
	{
		if (isNull())
		{
			throw HibernateException("Could not save or update from null session.");
		}

		if (qobject.isNull())
		{
			throw HibernateException("Could not save or update null object.");
		}

		QVariant idValue = getIdValue(qobject); 
		StrongPointer<QObject> controlObject = get(*(qobject->metaObject()), idValue);

		if (controlObject.isNull())
		{
			save(qobject);
		}
		else
		{
			update(qobject);
		}
	}

	void Session::setData(SessionData* data)
	{
		if (0 != mData)
		{
			mData->removeReference();
			if (!mData->hasReference())
			{
				close();
			}
			SessionData::destroy(mData);
			mData = 0;
		}

		mData = data;

		if (0 != mData)
		{
			mData->addReference();
		}
	}

	bool Session::isNull() const
	{
		return (0 == mData);
	}

	const Class& Session::getClassMapping( const QString& className ) const
	{
		if (isNull())
		{
			throw HibernateException("Could not get class mapping form null session.");
		}

		const SessionFactory& sessionFactory = getSessionFactory();
		return sessionFactory.getClassMapping(className);
	}

	void Session::saveInternal(const QMetaObject& metaObject, StrongPointer<QObject> qobject)
	{
		QSharedPointer<QSqlTableModel> sqlTableModel = createSqlTableModel(metaObject);	
		QString className = metaObject.className();
		QSqlRecord sqlRecord = sqlTableModel->record();

		QObjectPersister persister(className, *this);
		sqlRecord = persister.serialize(qobject, sqlRecord);

		qDebug() << sqlRecord.value("id");

		sqlTableModel->insertRecord(-1, sqlRecord);
		bool result = sqlTableModel->submitAll();
		checkSqlTableModelError(sqlTableModel);	

		qDebug() << sqlRecord.value("id");


		QVariant idValue = sqlTableModel->query().lastInsertId();
		//	sqlRecord.setValue(idColumn, idValue); 

		persister.deserializeIdentifier(idValue, qobject);
		qDebug() << sqlRecord.value("id");

		persister.applyOneToOne(qobject);
		persister.applyList(qobject);
		//persister.applyJoinedSubclass(qobject);
		//TODO: deserialize identifier if id is generated.
	}

	QString Session::getIdentifierName(StrongPointer<QObject> qobject)
	{
		QString className = qobject->metaObject()->className();
		Class classMapping = getClassMapping(className);	

		if (classMapping.isSubclass())
		{
			className = qobject->metaObject()->superClass()->className();
			classMapping = getClassMapping(className);
		}

		QString idName = classMapping.getIdentifierMapping().getName();
		return idName;
	}

	void Session::updateInternal(const QMetaObject& metaObject, StrongPointer<QObject> qobject)
	{
		QSharedPointer<QSqlTableModel> sqlTableModel = createSqlTableModel(metaObject);

		QString idName = getIdentifierName(qobject);
		QVariant idValue = qobject->property(idName.toLocal8Bit());	

		Class classMapping = getClassMapping(metaObject.className());
		QString filter = QtSqlHelper::getFilter(classMapping, idValue);

		sqlTableModel->setFilter(filter); 
		sqlTableModel->select();
		checkSqlTableModelError(sqlTableModel);

		if (sqlTableModel->rowCount() == 0)
		{
			throw HibernateException("Could not found row to update.");	
		}

		QSqlRecord sqlRecord = sqlTableModel->record(0);
		QObjectPersister persister(classMapping.getClassName(), *this);

		sqlRecord = persister.serialize(qobject, sqlRecord, true);

		bool result = sqlTableModel->setRecord(0, sqlRecord);
		result = sqlTableModel->submitAll();
		checkSqlTableModelError(sqlTableModel);

		persister.applyOneToOne(qobject);
		persister.applyList(qobject);
	}

	void Session::deleteInternal(const QMetaObject& metaObject, StrongPointer<QObject> object )
	{
		QSharedPointer<QSqlTableModel> sqlTableModel = createSqlTableModel(metaObject);

		const SessionFactory& sessionFactory = mData->getSessionFactory();
		const Class& classMapping = sessionFactory.getClassMapping(metaObject.className());

		QString identifierName = classMapping.getIdentifierMapping().getName();
		QVariant value = object->property(identifierName.toLocal8Bit());

		QString idColumn = classMapping.getIdentifierMapping().getColumnMappings().at(0).getName();
		sqlTableModel->setFilter(idColumn + " = " + value.toString()); 
		sqlTableModel->select();
		checkSqlTableModelError(sqlTableModel);

		if (sqlTableModel->rowCount() == 0)
		{
			throw HibernateException("Could not found row to delete.");	
		}

		sqlTableModel->removeRow(0);
		bool result = sqlTableModel->submitAll();
		checkSqlTableModelError(sqlTableModel);
	}
}
