#include <QHibernate/Query.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/Class.h>
#include <QHibernate/Session.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/QObjectPersister.h>
#include <QHibernate/QHibernate.h>
#include <QHibernate/QueryData.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>

#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

namespace QHibernate
{
	Query::Query(
		const Session& session,
		const QString& queryString)
		: mData(0)
	{
		setData(new QueryData(session));
		mData->setQueryString(queryString);
	}

	Query::Query(const Query& query)
		: mData(0)
	{
		*this = query;
	}

	Query::~Query()
	{
		setData(0);
	}

	QList<StrongPointer<QObject>> Query::list()
	{
		return getResultSet();
	}

	QList<StrongPointer<QObject>> Query::getResultSet()
	{
		QRegExp regExp("from\\s+(\\S+)(\\s+where\\s+(\\S+)\\s+(!=)\\s+(\\S+))?(\\s+order by (\\w+))?");

		regExp.setCaseSensitivity(Qt::CaseInsensitive);

		QString queryString = mData->getQueryString();
		if(regExp.indexIn(queryString) < 0)
		{
			throw HibernateException("Invalid query string syntax.");
		}

		QStringList tokens = regExp.capturedTexts();
		QString className = regExp.cap(1);



		Session session = mData->getSession();
		const Class& classMapping = session.getSessionFactory().getClassMapping(className);
		QString tableName = classMapping.getTableName();

		QSqlTableModel sqlTableModel(0, session.getConnection());

		sqlTableModel.setTable(classMapping.getTableName());	
		QSqlError error = sqlTableModel.lastError();

		if (error.isValid())
		{
			throw HibernateException(error.text());
		}

		if (!tokens.at(3).isEmpty())
		{
			QString propertyName = tokens.at(3);
			QString operand = tokens.at(4);
			QString value = tokens.at(5);

			PropertyMapping property = classMapping.getProperty(propertyName);
			if (property.getName().isEmpty())
			{
				throw HibernateException(
					QString("Could not find property(%1) in where clause.")
					.arg(propertyName));
			}

			QString columnName = property.getColumn(0).getName();

			sqlTableModel.setFilter(QString("%1 %2 %3")
				.arg(columnName)
				.arg(operand)
				.arg(value));
		}

		if (regExp.captureCount() > 6)
		{
			QString orderProperty = regExp.cap(7);

			const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();
			if (idMapping.getName() == orderProperty)
			{
				QString columName = idMapping.getColumnMappings().at(0).getName();
				int fieldIndex = sqlTableModel.fieldIndex(columName);
				sqlTableModel.setSort(fieldIndex, Qt::AscendingOrder);
			}
		}


		bool result = sqlTableModel.select();
		if (!result)
		{
			QSqlError error = sqlTableModel.lastError();

			if (error.isValid())
			{
				throw HibernateException(error.text());
			}
		}

		QList<StrongPointer<QObject>> objectList;
		QObjectPersister persister(className, mData->getSession());

		qint32 rowCount = sqlTableModel.rowCount();

		for(qint32 i = 0; i < rowCount; ++i)
		{
			QSqlRecord sqlRecord = sqlTableModel.record(i);

			for (int j = 0; j < sqlRecord.count(); ++j)
			{
				QVariant value = sqlRecord.value(j);
				qDebug() << sqlRecord.fieldName(j) << ":" << value;
			}
			qDebug() << "\n";

			StrongPointer<QObject> qobject = persister.deserialize(sqlRecord);

			if (!qobject.isNull())
			{
				objectList.push_back(qobject);
			}
		}

		return objectList;
	}


	const QString& Query::getQueryString() const
	{
		return mData->getQueryString();
	}

	Query& Query::operator=(const Query& query)
	{
		setData(query.mData);
		return *this;
	}

	void Query::setData(QueryData* data)
	{
		if (0 != mData)
		{
			mData->removeReference();
			QueryData::destroy(mData);
			mData = 0;
		}

		mData = data;
		if (0 != mData)
		{
			mData->addReference();
		}
	}
}

