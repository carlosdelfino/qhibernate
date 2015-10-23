
#ifndef QHibernate_SessionFactory
#define QHibernate_SessionFactory

#include <QHibernate/QHibernate_global.h>
#include <QHibernate/Class.h>
#include <QHibernate/Session.h>
#include <QHibernate/Pointer.h>

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QSharedPointer>

namespace QHibernate
{
	class SessionFactoryData;

	class QHIBERNATE_API SessionFactory
	{
	public:

		SessionFactory();

		SessionFactory(const SessionFactory& sessionFactory);

		~SessionFactory();

		SessionFactory& operator = (const SessionFactory& sessionFactory);

		void close();

		Session openSession();

		//These methods are not in orginal Hibernate API

		void setDriverName(const QString& driverName);

		QString getDriverName() const;

		void setDatabaseName(const QString& databaseName);

		QString getDatabaseName() const;

		void setHostName(const QString& hostName);

		QString getHostName() const;

		void setUserName(const QString& userName);

		QString getUserName() const;

		void setPassword(const QString& password);

		QString getPassword() const;

		const Class& getClassMapping(const QString& className) const;

		const Class& getClassMapping(const Pointer<QObject>& qobject) const;

		bool hasClassMapping(const QString& className) const;

		void addClassMapping(const Class& classMapping);

		bool isValid() const;

		bool isNull() const;

	private:

		friend class Configuration;

		SessionFactory(const Configuration& configuration);

	private:

		void setData(SessionFactoryData* data);

		SessionFactoryData* mData;
	};
}

#endif
