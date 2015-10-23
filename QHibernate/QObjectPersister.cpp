
#include <QHibernate/QObjectPersister.h>
#include <QHibernate/Class.h>
#include <QHibernate/HibernateException.h>
#include <QHibernate/QHibernate.h>
#include <QHibernate/ObjectListProxy.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/ObjectProxy.h>
#include <QHibernate/IdentifierGeneratorFactory.h>
#include <QHibernate/IdentifierGenerator.h>
#include <QHibernate/CustomUserType.h>
#include <QHibernate/Delete.h>
#include <QHibernate/Insert.h>
#include <QHibernate/Select.h>
#include <QHibernate/IdentifierMapping.h>

#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

namespace QHibernate
{
	QObjectPersister::QObjectPersister(const QString& className, const Session& session)
		: mClassName(className)
		, mSession(session)
		, mMetaObject(MetaObjectRegistery::getInstance().getMetaObject(className))
	{
	}

	StrongPointer<QObject> QObjectPersister::deserialize(const QSqlRecord& sqlRecord)
	{	
		QObject* objectPointer = 0;
		deserialize(sqlRecord, objectPointer);
		return StrongPointer<QObject>(objectPointer);
	}

	QObject* QObjectPersister::createObjectForRecord(const QSqlRecord& sqlRecord)
	{
		QObject* emptyObject = 0;

		const Class& classMapping = getClassMapping();

		if (!classMapping.hasSubclass())
		{
			emptyObject = mMetaObject.newInstance();
		}
		else
		{
			ClassList joinedSubclassList = classMapping.getJoinedSubclassList();
			QSqlQuery sqlQuery(mSession.getConnection());

			bool looking = true;
			qint32 index = 0;
			while(looking) 
			{
				if (index < joinedSubclassList.size())
				{
					Class joinedSubclass = joinedSubclassList.at(index);

					QString className = joinedSubclass.getClassName();
					const QMetaObject& metaObject 
						= MetaObjectRegistery::getInstance().getMetaObject(className);		

					QString queryString = buildSelectQueryForJoinedSubclass(joinedSubclass, classMapping, sqlRecord);

					sqlQuery.prepare(queryString);

					bool result = sqlQuery.exec();
					if (result)
					{
						if (1 == sqlQuery.numRowsAffected())
						{
							sqlQuery.first();
							QSqlRecord joinedRecord = sqlQuery.record();

							for (int i = 0; i < joinedRecord.count(); ++i)
							{
								qDebug() << joinedRecord.field(i).name() << "=" << joinedRecord.value(i);
							}
							emptyObject = metaObject.newInstance();

							PropertyList propertyList = joinedSubclass.getProperties();
							deserializeProperties(propertyList, joinedRecord, emptyObject);
							looking = false;
						}
					}
				}
				else
				{
					looking = false;
				}
				index++;
			}	
		}

		return emptyObject;
	}

	void QObjectPersister::deserialize(const QSqlRecord& sqlRecord, 
		QObject*& object )
	{
		object = createObjectForRecord(sqlRecord);

		if (0 == object)
		{
			throw HibernateException("Could not create object from QMetaObject.");
		}

		const Class& classMapping = getClassMapping();
		const QVector<PropertyMapping>& properties = classMapping.getProperties();
		deserializeProperties(properties, sqlRecord, object);


		const IdentifierMapping& identityMapping = classMapping.getIdentifierMapping();
		const QVector<ColumnMapping>& columnMappings = identityMapping.getColumnMappings();
		QString idName = identityMapping.getName();
		for (qint32 c = 0; c < columnMappings.size(); ++c)
		{
			const ColumnMapping& columnMapping = columnMappings[c];
			QString columnName = columnMapping.getName();
			QVariant value = sqlRecord.value(columnName);
			object->setProperty(idName.toLocal8Bit(), value);
		}

		const ListMappingList& listMappingList = classMapping.getListMappingList();
		foreach (ListMapping listMapping, listMappingList)
		{
			QByteArray name = listMapping.getName().toLocal8Bit();

			ObjectListProxy objectList(mSession, listMapping, object->property(idName.toLocal8Bit()));
			bool result = object->setProperty(name.data(), 
				QVariant::fromValue(objectList));

			if (!result)
			{
				delete object;
				object = 0;

				throw HibernateException(
					QString("Could not set value to property(%1).")					
					.arg(name.constData()));
			}
		}

		const ManyToOneList& manyToOneList = classMapping.getManyToOneList();
		foreach(const ManyToOne& manyToOne, manyToOneList)
		{
			QString foreignKey = manyToOne.getColumnMapingList().at(0).getName();
			if (!sqlRecord.isNull(foreignKey))
			{
				QVariant filterValue = sqlRecord.value(foreignKey);
				ObjectProxy* objectProxy = new ObjectProxy(mSession, manyToOne, filterValue);

				QByteArray name = manyToOne.getName().toLocal8Bit();

				int userType = object->property(name).userType();
				bool result = object->setProperty(name, 
					QVariant(userType, 
					new StrongPointer<QObject>(objectProxy)));

				if (!result)
				{
					delete object;
					object = 0;

					throw HibernateException(
						QString("Could not set value to property(%1).")					
						.arg(name.constData()));
				}
			}
		}

		const OneToOneList& oneToOneList = classMapping.getOneToOneList();
		foreach(const OneToOne& oneToOne, oneToOneList)
		{
			QVariant idValue = object->property(idName.toLocal8Bit());
			ObjectProxy* objectProxy = new ObjectProxy(mSession, oneToOne, idValue);

			QByteArray propertyName = oneToOne.getName().toLocal8Bit();
			int userType = object->property(propertyName).userType();

			StrongPointer<QObject>* value = 
				new StrongPointer<QObject>(objectProxy);

			bool result = 
				object->setProperty(
				propertyName, 
				QVariant(userType, value));

			if (!result)
			{
				delete value;		

				throw HibernateException(
					QString("Could not set value to property(%1).")					
					.arg(oneToOne.getName()));
			}
		}

	}

	QSqlRecord QObjectPersister::serialize(const Pointer<QObject>& qobject, 
		const QSqlRecord& sqlRecord, 
		bool forUpdate)
	{
		QSqlRecord result = sqlRecord;

		const Class& classMapping = getClassMapping();

		if (classMapping.isSubclass())
		{
			KeyMapping key = classMapping.getKey();
			QString keyName = key.getColumn(0).getName();
			QVariant value = qobject->property(keyName.toLocal8Bit());
			result.setValue(keyName, value);
		}
		else
		{
			const IdentifierMapping& identifierMapping = classMapping.getIdentifierMapping();
			serializeIdentifier(identifierMapping, qobject, result, forUpdate);
		}

		const PropertyList& properties = classMapping.getProperties();
		foreach (const PropertyMapping& property, properties)
		{
			try
			{
				serializeProperty(property, qobject, result);
			}
			catch(HibernateException exception)
			{
				QString message = exception.getMessage();
				throw HibernateException(message + QString(" Class name is '%1'.").arg(classMapping.getClassName()));
			}

		}

		const ManyToOneList& manyToOnes = classMapping.getManyToOneList();
		foreach (ManyToOne manyToOne, manyToOnes)
		{
			serializeManyToOne(manyToOne, qobject, result);
		}

		return result;
	}

	void QObjectPersister::serializeIdentifier(const IdentifierMapping& identifierMapping, 
		const Pointer<QObject>& object,
		QSqlRecord& sqlRecord,
		bool forUpdate)
	{
		if (!forUpdate)
		{
			GeneratorMapping generatorMapping = identifierMapping.getGeneratorMapping();
			QString name = generatorMapping.getName();

			IdentifierGenerator* generator 
				= IdentifierGeneratorFactory::getInstance().getGenerator(name);

			if (0 == generator)
			{
				throw HibernateException(
					QString("Could not found generator(%1)")
					.arg(name));
			}

			generator->serialize(mSession, 
				object, 
				sqlRecord, 
				identifierMapping,
				generatorMapping);
		}
		// 	else
		// 	{
		// 		serializeProperty(identifierMapping, object, sqlRecord);
		// 	}	
	}

	void QObjectPersister::serializeProperty(const PropertyMapping& propertyMapping, 
		const Pointer<QObject>& object, 
		QSqlRecord& sqlRecord)
	{

		QVariant value = object->property(propertyMapping.getName().toLocal8Bit());

		const QVector<ColumnMapping>& columnMappings = propertyMapping.getColumnMappings();
		if (columnMappings.size() == 1)
		{
			QString propertyType = propertyMapping.getType();
			const ColumnMapping& columnMapping = columnMappings.at(0);
			QString columnName = columnMapping.getName();

			QSqlField sqlField = sqlRecord.field(columnName);

			if (!sqlField.isValid())
			{
				throw HibernateException(QString("Could not map the column(%1) for the property(%2).")
					.arg(columnName)
					.arg(propertyMapping.getName()));
			}

			if (CustomUserTypeRegistery::hasType(propertyType))
			{
				const CustomUserType* customUserType = 
					CustomUserTypeRegistery::getCustomUserType(propertyType);		

				QVariant columnValue = customUserType->getPropertyValue(value, 0);
				sqlRecord.setValue(columnName, columnValue);
			}
			else
			{			
				sqlRecord.setValue(columnName, value);
			}
		}
		else if (columnMappings.size() > 1)
		{
			QString typeName = propertyMapping.getType();
			const CustomUserType* customUserType = 
				CustomUserTypeRegistery::getCustomUserType(typeName);

			if (0 != customUserType)
			{
				for (qint32 i = 0; i < columnMappings.size(); ++i)
				{
					const ColumnMapping& columnMapping = columnMappings[i];
					QVariant columnValue = customUserType->getPropertyValue(value, i);
					sqlRecord.setValue(columnMapping.getName(), columnValue);
				}
			}
		}
	}

	const Class& QObjectPersister::getClassMapping() const
	{
		const SessionFactory& factory = mSession.getSessionFactory();
		const Class& classMapping = factory.getClassMapping(mClassName);
		return classMapping;
	}

	Q_DECLARE_METATYPE(StrongPointer<QObject>);

	void QObjectPersister::serializeManyToOne(
		const ManyToOne& manyToOne, 
		const Pointer<QObject>& object, 
		QSqlRecord& sqlRecord )
	{
		const QList<ColumnMapping>& columnMappingList = manyToOne.getColumnMapingList();
		QString propertyName = manyToOne.getName();

		if (object->metaObject()->indexOfProperty(propertyName.toLocal8Bit()) == -1)
		{
			throw HibernateException(QString("Could not find property('%1') on object('%2').")
				.arg(propertyName)
				.arg(object->metaObject()->className()));
		}


		QVariant value = object->property(propertyName.toLocal8Bit());

		if (!value.isNull())
		{
			QString valueType = value.typeName();
			const void* valuePointer = value.constData();
			StrongPointer<QObject> childObject = *reinterpret_cast<const Pointer<QObject>*>(valuePointer);

			if (!childObject.isNull())
			{
				Cascade cascade = manyToOne.getCascade();
				if ((cascade & eCascadeSaveUpdate) == eCascadeSaveUpdate)
				{
					mSession.saveOrUpdate(childObject);
				}

				QString childClassName = childObject->metaObject()->className();
				qDebug() << childClassName;
				const Class& childClassMapping = mSession.getClassMapping(childClassName);
				const IdentifierMapping& childIdMapping = childClassMapping.getIdentifierMapping();
				const QVector<ColumnMapping>& childColumnMappingList = childIdMapping.getColumnMappings();

				if (childColumnMappingList.size() != columnMappingList.size())
				{
					throw HibernateException("Number of columns mismatched.");
				}

				for(qint32 i = 0; i < columnMappingList.size(); ++i)
				{
					const ColumnMapping& columnMapping = columnMappingList.at(i);
					const ColumnMapping& childColumnMapping = childColumnMappingList.at(i);

					QString idName = childColumnMapping.getName();
					QVariant value = childObject->property(idName.toLocal8Bit().constData());

					QString foreignName = columnMapping.getName();
					sqlRecord.setValue(foreignName, value);
				}
			}
			else
			{

			}
		}
	}

	void QObjectPersister::serializeOneToOne(
		const OneToOne& oneToOne, 
		const Pointer<QObject>& object)
	{
		if (eCascadeSaveUpdate  == (oneToOne.getCascade() & eCascadeSaveUpdate))
		{
			QByteArray propertyName = oneToOne.getName().toLocal8Bit();
			QVariant value = object->property(propertyName);
			qDebug() << value.typeName();

			StrongPointer<QObject> childObject = *reinterpret_cast<const StrongPointer<QObject>*>(value.constData());

			if (!childObject.isNull())
			{
				mSession.saveOrUpdate(childObject);
			}
		}
	}

	void QObjectPersister::deserializeIdentifier(
		const QVariant& idValue, 
		StrongPointer<QObject> qobject )
	{
		if (!qobject.isNull())
		{
			const Class& classMapping = mSession.getClassMapping(mClassName);
			const IdentifierMapping& idMapping = classMapping.getIdentifierMapping();
			QByteArray idName = idMapping.getName().toLocal8Bit();		
			qobject->setProperty(idName, idValue);
		}
	}

	void QObjectPersister::applyOneToOne(const Pointer<QObject>& qobject)
	{	
		const Class& classMapping = mSession.getClassMapping(mClassName);
		OneToOneList oneToOnes = classMapping.getOneToOneList();

		foreach(OneToOne oneToOne, oneToOnes)
		{
			serializeOneToOne(oneToOne, qobject);
		}
	}

	void QObjectPersister::applyList(const Pointer<QObject>& qobject)
	{
		const Class& classMapping = mSession.getClassMapping(mClassName);
		const ListMappingList listMappingList = classMapping.getListMappingList();

		foreach(const ListMapping& listMapping, listMappingList)
		{
			serializeList(listMapping, qobject);
		}
	}

	void QObjectPersister::serializeList(const ListMapping& listMapping, 
		const Pointer<QObject>& qobject)
	{
		if (listMapping.getManyToMany().isValid())
		{
			serializeListManyToMany(listMapping, qobject);
		}
		else if (listMapping.getOneToMany().isValid())
		{
			serializeListOneToMany(listMapping, qobject);
		}
	}

	void QObjectPersister::serializeListManyToMany( const ListMapping &listMapping, const Pointer<QObject>& qobject )
	{
		QString name = listMapping.getName();
		QVariant value = qobject->property(name.toLocal8Bit());
		ObjectListProxy objectListProxy = value.value<ObjectListProxy>();

		Delete del;
		del.setTableName(listMapping.getTable());

		const Class& classMapping = mSession.getClassMapping(mClassName);
		IdentifierMapping idMapping = classMapping.getIdentifierMapping();
		QVariant idValue = qobject->property(idMapping.getName().toLocal8Bit());

		QString idColumn = listMapping.getKeyMapping().getColumnList().at(0).getName();


		del.addPrimaryKeyColumn(idColumn, idValue.toString());

		QString statement = del.getStatementString();

		QSqlQuery sqlQuery(mSession.getConnection());
		bool success = sqlQuery.prepare(statement);	 
		if (success)
		{
			success = sqlQuery.exec();
			if (!success)
			{
				throw HibernateException(sqlQuery.lastError().text());
			}		
		}
		else
		{
			throw HibernateException(sqlQuery.lastError().text());
		}

		QList<StrongPointer<QObject>> objectList = objectListProxy;

		const ManyToMany& manyToMany = listMapping.getManyToMany();
		const QVector<ColumnMapping> columnMappings = manyToMany.getColumnMappings();
		QString className = manyToMany.getClassName();
		const Class classMapping2 = mSession.getClassMapping(className);
		const IdentifierMapping& idMapping2 = classMapping2.getIdentifierMapping();
		const QVector<ColumnMapping>& columnMappings2 = idMapping2.getColumnMappings();

		qint32 columnCount = columnMappings.size();
		if (columnMappings.size() != columnMappings2.size())
		{
			throw HibernateException("Columns not mapping.");
		}

		for (qint32 i = 0; i < objectList.size(); ++i)
		{
			StrongPointer<QObject> subObject = objectList.at(i);

			Insert insert;
			insert.setTableName(listMapping.getTable());

			QString column = listMapping.getKeyMapping().getColumnList().at(0).getName();
			QString stringValue = idValue.toString();
			insert.addColumn(column, stringValue);


			for (qint32 columnIndex = 0; 
				columnIndex < columnCount; 
				++columnIndex)
			{
				QString subColumn = columnMappings2.at(columnIndex).getName();
				QVariant value = subObject->property(subColumn.toLocal8Bit());

				QString column = columnMappings.at(columnIndex).getName();
				insert.addColumn(column, value.toString());
			}

			statement = insert.getStatementString();

			success = sqlQuery.prepare(statement);
			if (success)
			{
				success = sqlQuery.exec();
				if (!success)
				{
					throw HibernateException(sqlQuery.lastError().text());
				}		
			}
			else
			{
				throw HibernateException(sqlQuery.lastError().text());
			}
		}
	}

	void QObjectPersister::serializeListOneToMany( const ListMapping& listMapping, 
		const Pointer<QObject>& qobject )
	{
		const Class& classMapping = mSession.getClassMapping(mClassName);

		QString name = listMapping.getName();
		QVariant value = qobject->property(name.toLocal8Bit());
		ObjectListProxy objectListProxy = value.value<ObjectListProxy>();

		QList<StrongPointer<QObject>> objectList = objectListProxy;

		foreach(StrongPointer<QObject> subObject, objectList)
		{
			mSession.saveOrUpdate(subObject);
		}
	}

	void QObjectPersister::deserializeProperties(const PropertyList& properties, 
		const QSqlRecord &sqlRecord, 
		QObject*& object )
	{
		for (qint32 i = 0; i < properties.size(); ++i)
		{
			const PropertyMapping& property = properties[i];
			deserializeProperty(property, sqlRecord, object);
		}
	}

	void QObjectPersister::deserializeProperty(const PropertyMapping &property, 
		const QSqlRecord &sqlRecord, 
		QObject*& object)
	{
		QString propertyName = property.getName();

		const QVector<ColumnMapping>& columnMappings = property.getColumnMappings();
		if (1 == columnMappings.length())
		{
			const ColumnMapping& columnMapping = columnMappings[0];
			QVariant value = sqlRecord.value(columnMapping.getName());

			QString propertyType = property.getType();

			if (CustomUserTypeRegistery::hasType(propertyType))
			{
				const CustomUserType* customUserType = 
					CustomUserTypeRegistery::getCustomUserType(propertyType);

				QVariant component = object->property(propertyName.toLocal8Bit());			
				customUserType->setPropertyValue(component, 0, value);
				object->setProperty(propertyName.toLocal8Bit(), component);

			}
			else
			{
				object->setProperty(propertyName.toLocal8Bit(), value);
			}
		}
		else if (columnMappings.length() > 1)
		{
			QString typeName = property.getType();
			const CustomUserType* customUserType = 
				CustomUserTypeRegistery::getCustomUserType(typeName);

			if (0 != customUserType)
			{
				QVariant component = object->property(propertyName.toLocal8Bit());

				for (qint32 p = 0; p < columnMappings.size(); ++p)
				{
					const ColumnMapping& columnMapping = columnMappings[p];
					QVariant value = sqlRecord.value(columnMapping.getName());


					customUserType->setPropertyValue(component, p, value);
					object->setProperty(propertyName.toLocal8Bit(), component);
				}
			}
		}
	}

	QString QObjectPersister::buildSelectQueryForJoinedSubclass(const Class &joinedSubclass, 
		const Class &classMapping, 
		const QSqlRecord &sqlRecord)
	{
		QString tableName = joinedSubclass.getTableName();
		Select select;
		select.setFromClause(tableName);

		KeyMapping key = joinedSubclass.getKey();
		IdentifierMapping identifier = classMapping.getIdentifierMapping();

		if (key.getColumnCount() != identifier.getColumnCount())
		{
			throw HibernateException("");
		}
		qint32 columnCount = key.getColumnCount();
		QString whereClause;
		for(qint32 columnIndex = 0; columnIndex < columnCount; ++columnIndex)
		{
			ColumnMapping idColumn = identifier.getColumn(columnIndex);
			ColumnMapping keyColumn = key.getColumn(columnIndex);

			QVariant value = sqlRecord.value(idColumn.getName());
			if (!value.isNull())
			{
				if (!whereClause.isEmpty())
				{
					whereClause.append(" AND ");
				}
				whereClause.append(keyColumn.getName());
				whereClause.append(" = ");
				whereClause.append(value.toString());
			}
		}

		select.setWhereClause(whereClause);

		QString queryString = select.getStatementString();

		return queryString;
	}

	// void QObjectPersister::applyJoinedSubclass(const Pointer<QObject>& qobject)
	// {
	// 	const QMetaObject* metaObject = qobject->metaObject();
	// 	const QMetaObject* superMetaObject = metaObject->superClass();
	// 
	// 	if (isValid(superMetaObject))
	// 	{
	// 		QString superClassName = superMetaObject->className();
	// 
	// 		const SessionFactory& sessionFactory = mSession.getSessionFactory();
	// 		if (sessionFactory.hasClassMapping(superClassName))
	// 		{
	// 			const Class& classMapping = mSession.getClassMapping(superClassName);
	// 
	// 			JoinedSubclass joinedSubclass;
	// 			JoinedSubclassList joinedSubclassList = classMapping.getJoinedSubclassList();
	// 			QString className = metaObject->className();
	// 			foreach(JoinedSubclass current, joinedSubclassList)
	// 			{
	// 				if (current.getClassName() == className)
	// 				{
	// 					joinedSubclass = current;
	// 				}
	// 			}
	// 
	// 			if (joinedSubclass.isValid())
	// 			{
	// 				PropertyList propertyList = joinedSubclass.getPropertyList();
	// 				foreach (PropertyMapping property, propertyList)
	// 				{
	// 					//QString queryString = buildSelectQueryForJoinedSubclass(joinedSubclass, classMapping, );
	// 
	// 					//mSession.saveOrUpdate();
	// 				}
	// 			}
	// 		}
	// 	}
	// }
}

