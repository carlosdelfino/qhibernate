
#ifndef QHibernate_OneToOne
#define QHibernate_OneToOne

#include <QHibernate/Cascade.h>
#include <QHibernate/OuterJoin.h>
#include <QHibernate/Fetch.h>
#include <QHibernate/Lazy.h>

#include <QtCore/QString>

namespace QHibernate
{
	class OneToOneData;

	class OneToOne
	{
	public:

		OneToOne();

		OneToOne(const QString& name);

		OneToOne(const OneToOne& oneToOne);

		~OneToOne();

		OneToOne& operator = (const OneToOne& oneToOne);

		QString getName() const;

		void setClassName(const QString& className);
		QString getClassName() const;

		void setCascade(Cascade cascade);
		Cascade getCascade() const;

		void setLazy(Lazy lazy);
		Lazy getLazy() const;

		bool isValid() const;

	private:

		OneToOneData* mData;

		void setData(OneToOneData* data);
	};
}

#endif
