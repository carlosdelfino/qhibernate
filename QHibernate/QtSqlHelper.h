
#ifndef QHibernate_QtSqlHelper
#define QHibernate_QtSqlHelper

#include <QtSql/QSqlRecord>

#include <QtCore/QVariant>
#include <QtCore/QString>


class QSqlTableModel;
class QVariant;

namespace QHibernate
{
	class Session;
	class Class;


	class QtSqlHelper
	{
	public:

		static QSqlRecord getRecord(const Session& session, 
			const QString& className, 
			const QVariant& id);

		static void deleteRecord(const Session& session,
			const QString& className);

		static QString getFilter(const Class& classMapping, 
			const QVariant& idValue);

	private:

		static void checkError(const QSqlTableModel& model);

	};

}

#endif
