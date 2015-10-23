
#ifndef QHibernate_Session
#define QHibernate_Session

#include <QHibernate/QHibernate_global.h>
#include <QHibernate/Transaction.h>
#include <QHibernate/Query.h>
#include <QHibernate/Criteria.h>

#include <QHibernate/StrongPointer.h>
#include <QtCore/QSharedPointer>

class QSqlDatabase;
class QSqlRecord;
class QSqlTableModel;

namespace QHibernate
{
	class SessionData;
	class SessionFactory;
	class IdentifierMapping;
	class PropertyMapping;
	class Class;

	class QHIBERNATE_API Session
	{
	public:

		Session();

		Session(const Session& session);

		~Session();

		Session& operator = (const Session& session);

		void open();

		void close();

		void save(StrongPointer<QObject> qobject);

		void update(StrongPointer<QObject> qobject);

		void saveOrUpdate(StrongPointer<QObject> qobject);

		void erase(StrongPointer<QObject> object);


		StrongPointer<QObject> get(const QMetaObject& metaObject, const QVariant& id);

		template<class ObjectType> Pointer<ObjectType> get(const QVariant& id)
		{
			return get(ObjectType::staticMetaObject, id);
		}

		void beginTransaction();

		Transaction getTransaction() const;	

		Query createQuery(const QString& queryString);

		Criteria createCriteria();

		const SessionFactory& getSessionFactory() const;

		const QSqlDatabase& getConnection() const;

		bool isNull() const;

		const Class& getClassMapping(const QString& className) const;

	private:

		friend class SessionFactory;

		Session(const SessionFactory& sessionFactory);

	private:

		SessionData* mData;

		void setData(SessionData* data);		

		void serializeIdentifier(const IdentifierMapping& identifierMapping, 
			const QObject& object,
			QSqlRecord& sqlRecord);

		void serializeProperty(const PropertyMapping& propertyMapping, 
			const QObject& value, 
			QSqlRecord& sqlRecord);

		QSharedPointer<QSqlTableModel> createSqlTableModel(const Pointer<QObject>& object);

		QSharedPointer<QSqlTableModel> createSqlTableModel(const QMetaObject& metaObject);

		void checkSqlTableModelError(QSharedPointer<QSqlTableModel> sqlTableModel);

		QVariant getIdValue(StrongPointer<QObject> qobject) const;

		void saveInternal(const QMetaObject& metaObject, StrongPointer<QObject> qobject);

		void updateInternal(const QMetaObject& metaObject, StrongPointer<QObject> qobject);

		QString getIdentifierName(StrongPointer<QObject> qobject);

		void deleteInternal(const QMetaObject& metaObject, 
			StrongPointer<QObject> object );
	};

}


#endif

