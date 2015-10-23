
#include <QHibernate/NativeGenerator.h>
#include <QHibernate/Class.h>
#include <QHibernate/Session.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtSql/QSqlRecord>

namespace QHibernate
{
	void NativeGenerator::serialize(
		const Session& session, 
		const Pointer<QObject>& object, 
		QSqlRecord& sqlRecord,
		const IdentifierMapping& idMapping,
		const GeneratorMapping& generatorMapping)
	{
		//QString className = object->metaObject()->className();
		//const Class& classMapping = session.getClassMapping(className);
		//const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();

		const QVector<ColumnMapping>& columnMappings = idMapping.getColumnMappings();
		foreach (ColumnMapping columnMapping, columnMappings)
		{
			QString name = columnMapping.getName();
			sqlRecord.setGenerated(name, false);
		}
	}
}

