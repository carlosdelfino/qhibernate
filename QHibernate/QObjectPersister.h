
#ifndef QHibernate_QObjectPersister
#define QHibernate_QObjectPersister

#include <QHibernate/Session.h>
#include <QHibernate/StrongPointer.h>

class QSqlRecord;
class QMetaObject;
class QObject;

namespace QHibernate
{
	class Class;
	class IdentifierMapping;
	class PropertyMapping;
	class ManyToOne;
	class OneToOne;
	class ListMapping;
	class JoinedSubclass;

	class QObjectPersister
	{
	public:

		QObjectPersister(const QString& className, 
			const Session& session);

		StrongPointer<QObject> deserialize(const QSqlRecord& sqlRecord);

		void deserialize(const QSqlRecord& sqlRecord, QObject*& object);

		void deserializeProperties( const QVector<PropertyMapping> &properties, const QSqlRecord &sqlRecord, QObject*& object );

		void deserializeProperty( const PropertyMapping &property, const QSqlRecord &sqlRecord, QObject*& object );
		QSqlRecord serialize(const Pointer<QObject>& qobject, 
			const QSqlRecord& sqlRecord, 
			bool forUpdate = false);	

		void deserializeIdentifier(const QVariant& idValue, 
			StrongPointer<QObject> qobject);

		void applyOneToOne(const Pointer<QObject>& qobject );

		void applyList(const Pointer<QObject>& qobject);

		//void applyJoinedSubclass(const Pointer<QObject>& qobject);

	private:

		QString mClassName;

		const QMetaObject& mMetaObject;

		const Class& getClassMapping() const;

		Session mSession;

		void serializeIdentifier(
			const IdentifierMapping& identifierMapping, 
			const Pointer<QObject>& object,
			QSqlRecord& sqlRecord,
			bool forUpdate);

		void serializeProperty(
			const PropertyMapping& propertyMapping, 
			const Pointer<QObject>& object, 
			QSqlRecord& sqlRecord);

		void serializeManyToOne(
			const ManyToOne& propertyMapping, 
			const Pointer<QObject>& object, 
			QSqlRecord& sqlRecord);

		void serializeOneToOne(const OneToOne& oneToOne, 
			const Pointer<QObject>& object//, 
			/*QSqlRecord& sqlRecord*/);

		void serializeList(const ListMapping& listMapping, 
			const Pointer<QObject>& qobject);

		void serializeListManyToMany( const ListMapping &listMapping, const Pointer<QObject>& qobject );
		void serializeListOneToMany( const ListMapping& listMapping, const Pointer<QObject>& qobject );

		QObject* createObjectForRecord(const QSqlRecord& sqlRecord);

		QString buildSelectQueryForJoinedSubclass(const Class& joinedSubclass, 
			const Class &classMapping, 
			const QSqlRecord &sqlRecord);
	};

}




#endif