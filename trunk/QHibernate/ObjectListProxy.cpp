
#include <QHibernate/ObjectListProxy.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/QObjectPersister.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/Select.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QHibernate/Dialect.h>

namespace QHibernate
{
	ObjectListProxy::ObjectListProxy()
		: mFetched(false)
	{
	}

	ObjectListProxy::ObjectListProxy(const Session& session, 
		const ListMapping& listMapping,
		const QVariant& filterValue)
		: mFetched(false)
		, mSession(session)
		, mListMapping(listMapping)
		, mFilterValue(filterValue)
	{
		if (!listMapping.isLazy())
		{
			fetchObjects();
			mSession = Session();
			mListMapping = ListMapping();
		}
	}

	void ObjectListProxy::checkFetching()
	{
		if (!mListMapping.isNull())
		{
			if (mListMapping.isLazy() && !mFetched)
			{
				fetchObjects();
			}
		}
	}

	void ObjectListProxy::checkSqlTableModelError( QSqlTableModel* model )
	{
		QSqlError error = model->lastError();
		if (error.isValid())
		{
			throw HibernateException(error.text());
		}
	}

	void ObjectListProxy::fetchObjects()
	{
		const OneToMany& oneToManyMapping = mListMapping.getOneToMany();
		if (oneToManyMapping.isValid())
		{
			fetchOneToMany(oneToManyMapping);
		}
		else
		{
			const ManyToMany& manyToMany = mListMapping.getManyToMany();
			fetchManyToMany(manyToMany);
		}
	}

	QString ObjectListProxy::buildQueryString()
	{
		Select select;
		QString selectClause;

		const ManyToMany& manyToMany = mListMapping.getManyToMany();
		QString className = manyToMany.getClassName();

		SessionFactory sessionFactory = mSession.getSessionFactory();
		const Class& classMapping = sessionFactory.getClassMapping(className);	

		{
			const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();
			const QVector<ColumnMapping>& columnsMapping = idMapping.getColumnMappings();

			foreach (const ColumnMapping& columnMapping, columnsMapping)
			{
				if (!selectClause.isEmpty())
				{
					selectClause.append(", ");
				}
				selectClause.append(classMapping.getTableName() + ".");
				selectClause.append(columnMapping.getName());
			}
		}

		const PropertyList& properties = classMapping.getProperties();
		foreach(const PropertyMapping& property, properties)
		{
			const QVector<ColumnMapping>& columnsMapping = property.getColumnMappings();
			foreach (const ColumnMapping& columnMapping, columnsMapping)
			{
				if (!selectClause.isEmpty())
				{
					selectClause.append(", ");
				}
				selectClause.append("\"" + columnMapping.getName() + "\"");
			}
		}

		select.setSelectClause(selectClause);

		QString fromClause = mListMapping.getTable() + ", " + classMapping.getTableName();  
		select.setFromClause(fromClause);

		QString joinClause;
		{
			const QVector<ColumnMapping> columnMappings = manyToMany.getColumnMappings();
			qint32 columnCount = columnMappings.size();

			const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();
			if (columnCount != classMapping.getIdentifierMapping().getColumnMappings().size())
			{
				throw HibernateException("Could not map many-to-many");
			}

			for (qint32 i = 0; i < columnMappings.size(); ++i)
			{
				const ColumnMapping& left = columnMappings[i];
				const ColumnMapping& right = idMapping.getColumnMappings().at(i);

				if (!joinClause.isEmpty())
				{
					joinClause.append(" and ");
				}
				joinClause.append(mListMapping.getTable());
				joinClause.append(".");
				joinClause.append(left.getName());
				joinClause.append(" = ");
				joinClause.append(classMapping.getTableName());
				joinClause.append(".");
				joinClause.append(right.getName());
			}
		}

		select.setOuterJoinsAfterWhere(joinClause);

		QString whereClause;

		const KeyMapping& keyMapping = mListMapping.getKeyMapping();
		whereClause.append(keyMapping.getColumnList().at(0).getName());
		whereClause.append(" = ");
		whereClause.append(mFilterValue.toString());
		select.setWhereClause(whereClause);

		return select.getStatementString();
	}

	void ObjectListProxy::fetchManyToMany(const ManyToMany& manyToMany)
	{
		QString className = manyToMany.getClassName();

		QSqlQuery sqlQuery(mSession.getConnection());
		QString queryString = buildQueryString();

		sqlQuery.prepare(queryString);
		bool result = sqlQuery.exec();

		QObjectPersister persister(className, mSession);

		while(sqlQuery.next())
		{

			QSqlRecord record = sqlQuery.record();
			StrongPointer<QObject> qobject = persister.deserialize(record);
			if (!qobject.isNull())
			{
				mObjectList.push_back(qobject);
			}
		}

		mFetched = true;
	}

	void ObjectListProxy::fetchOneToMany(const OneToMany& oneToMany)
	{
		QString className = oneToMany.getClassName();

		KeyMapping keyMapping = mListMapping.getKeyMapping();
		QString columnName = keyMapping.getColumnList().at(0).getName();

		QString tableName = mListMapping.getTable();

		QSqlTableModel model(0, mSession.getConnection());
		model.setTable(tableName);
		checkSqlTableModelError(&model);

		model.setFilter(QString("%1 = %2")
			.arg(columnName)
			.arg(mFilterValue.toString()));
		model.select();
		checkSqlTableModelError(&model);


		SessionFactory sessionFactory = mSession.getSessionFactory();
		const Class& classMapping = sessionFactory.getClassMapping(className);

		QObjectPersister persister(className, mSession);
		for(qint32 i = 0; i < model.rowCount(); ++i)
		{
			QSqlRecord record = model.record(i);
			StrongPointer<QObject> qobject = persister.deserialize(record);
			if (!qobject.isNull())
			{
				mObjectList.push_back(qobject);
			}
		}

		mFetched = true;
	}

}
