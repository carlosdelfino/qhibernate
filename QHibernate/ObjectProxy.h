
#ifndef QHibernate_ObjectProxy
#define QHibernate_ObjectProxy

#include <QHibernate/Session.h>
#include <QHibernate/ManyToOne.h>
#include <QHibernate/OneToOne.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>

namespace QHibernate
{
	class ObjectProxy
		: public PointerProxy
	{
	public:

		virtual ~ObjectProxy();

		virtual void* getPointer();

	private:

		friend class QObjectPersister;

		ObjectProxy(const Session& session, 
			const ManyToOne& manyToOne,
			const QVariant& filterValue);

		ObjectProxy(const Session& session,
			const OneToOne& oneToOne,
			const QVariant& filterValue);

	private:

		void fetchObject();

		ManyToOne mManyToOne;

		OneToOne mOneToOne;

		QVariant mFilterValue;

		Session mSession;
	};
}

#endif

