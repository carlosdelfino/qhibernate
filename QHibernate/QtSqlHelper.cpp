
#include <QHibernate/QtSqlHelper.h>
#include <QHibernate/Class.h>
#include <QHibernate/Session.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>

namespace QHibernate
{
	QSqlRecord QtSqlHelper::getRecord(
		const Session& session, 
		const QString& className, 
		const QVariant& idValue)
	{
		QSqlRecord sqlRecord;
		const Class& classMapping = session.getClassMapping(className);
		QString tableName = classMapping.getTableName();

		QSqlTableModel model(0, session.getConnection());
		model.setTable(tableName);
		checkError(model);

		QString filter = getFilter(classMapping, idValue);
		model.setFilter(filter);

		model.select();
		checkError(model);

		int rowCount = model.rowCount();
		if (1 == rowCount)
		{
			sqlRecord = model.record(0);
		}

		return sqlRecord;
	}

	void QtSqlHelper::checkError(const QSqlTableModel& model)
	{
		QSqlError error = model.lastError();
		if (error.isValid())
		{
			throw HibernateException(error.text());
		}
	}

	QString QtSqlHelper::getFilter(
		const Class& classMapping, 
		const QVariant& idValue)
	{
		bool ok = false;
		qint32 value = idValue.toInt(&ok);
		if (!ok)
		{
			//TODO: support composite keys and other types of keys.
			throw HibernateException("Unsupported primary key type.");
		}

		QString columnName;

		if (classMapping.isSubclass())
		{
			KeyMapping key = classMapping.getKey();
			columnName = key.getColumn(0).getName();
		}
		else
		{
			const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();
			columnName = idMapping.getColumnMappings().at(0).getName();
		}

		QString filter = columnName + "=" + QString::number(value);
		return filter;
	}
}

