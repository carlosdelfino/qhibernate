#include <QHibernate/ForeignGenerator.h>
#include <QHibernate/GeneratorMapping.h>
#include <QHibernate/Class.h>
#include <QHibernate/Session.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtCore/QDebug>
#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtSql/QSqlRecord>

namespace QHibernate
{
	void ForeignGenerator::serialize(const Session& session, 
		const Pointer<QObject>& object, 
		QSqlRecord& sqlRecord,
		const IdentifierMapping& idMapping,
		const GeneratorMapping& generatorMapping)
	{	
		QString propertyName = generatorMapping.getParamValue("property");
		QVariant value = object->property(propertyName.toLocal8Bit());

		const void* constData = value.constData();
		qDebug() << value.typeName();

		StrongPointer<QObject> childObject = *reinterpret_cast<const Pointer<QObject>*>(constData);
		if (childObject.isNull())
		{
			throw HibernateException("property is null");
		}

		QString className = childObject->metaObject()->className();
		const Class& classMapping = session.getClassMapping(className);

		QByteArray idName = classMapping.getIdentifierMapping().getName().toLocal8Bit();
		QVariant idValue = childObject->property(idName);

		className = object->metaObject()->className();
		const Class& classMapping2 = session.getClassMapping(className);

		QString idColumn = classMapping2.getIdentifierMapping().getColumnMappings().at(0).getName();
		sqlRecord.setValue(idColumn, idValue);
	}

}
