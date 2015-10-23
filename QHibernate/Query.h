
#ifndef QHibernate_Query
#define QHibernate_Query

#include <QHibernate/QHibernate_global.h>
#include <QHibernate/StrongPointer.h>

#include <QtCore/QObjectList>
#include <QtCore/QSharedPointer>

class QSqlTableModel;

namespace QHibernate
{
	class QueryData;
	
	class QHIBERNATE_API Query
	{
	public: 

		~Query();

		Query(const Query& query);

		Query& operator = (const Query& query);

		const QString& getQueryString() const;

		template <class ObjectType> QList<StrongPointer<ObjectType>> list()
		{
			QList<StrongPointer<QObject>> objectList = getResultSet();
			QList<StrongPointer<ObjectType>> newList;
			foreach(StrongPointer<QObject> object, objectList)
			{
				StrongPointer<ObjectType> castedObject = object;
				newList.push_back(castedObject);
			}

			return newList;
		}

		QList<StrongPointer<QObject>> list();

	private:

		friend class Session;

		Query(const Session& session, const QString& queryString);

	private:

		QueryData* mData;

		void setData(QueryData* data);

		QList<StrongPointer<QObject>> getResultSet();	


	};

}

#endif
