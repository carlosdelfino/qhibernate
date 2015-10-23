
#ifndef QHibernate_CsObjectList
#define QHibernate_CsObjectList

#include <QHibernate/ListMapping.h>
#include <QHibernate/Session.h>

#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QVariant>

class QObject;

namespace QHibernate
{

	class QHIBERNATE_API ObjectListProxy
	{
	public:

		ObjectListProxy();

		template<class T>
		operator QList<StrongPointer<T>> ()
		{
			return getList<T>();
		}

		template<class T>
		QList<StrongPointer<T>> getList() 
		{
			checkFetching();
			QList<StrongPointer<T>> list;
			foreach(StrongPointer<QObject> object, mObjectList)
			{
				list.push_back(object);
			}
			return list;
		}

		template<class T>
		ObjectListProxy& operator = (const QList<StrongPointer<T>>& list)
		{
			checkFetching();
			for (int i = 0; i < list.size(); ++i)
			{
				StrongPointer<QObject> qobject = list.at(i);
				mObjectList.push_back(qobject);
			}

			return *this;
		}

		template<class T> 
		void setList(const QList<T>& list)
		{
			mObjectList.clear();
			for (int i = 0; i < list.size(); ++i)
			{
				mObjectList.push_back(list.at(i));
			}
		}

	private:

		friend class QObjectPersister;

		ObjectListProxy(const Session& session, 
			const ListMapping& listMapping,
			const QVariant& filterValue);

	private:

		QSharedPointer<QObject> getNext();

		void checkSqlTableModelError(QSqlTableModel* model);

	private:

		void checkFetching();

		void fetchObjects();

		void fetchOneToMany(const OneToMany& oneToMany);

		void fetchManyToMany(const ManyToMany& manyToMany);

		QString buildQueryString();
	
		bool mFetched;

		ListMapping mListMapping;

		QList<StrongPointer<QObject>> mObjectList;

		Session mSession;

		QVariant mFilterValue;

	};

}

Q_DECLARE_METATYPE(QHibernate::ObjectListProxy);

#endif
