
#ifndef QHibernate_ManyToOne
#define QHibernate_ManyToOne

#include <QHibernate/Lazy.h>
#include <QHibernate/OuterJoin.h>
#include <QHibernate/Fetch.h>
#include <QHibernate/NotFound.h>
#include <QHibernate/ColumnMapping.h>
#include <QHibernate/Cascade.h>

#include <QtCore/QString>
#include <QtCore/QList>

namespace QHibernate
{
	struct ManyToOneData;

	class ManyToOne
	{
	public:

		ManyToOne();

		ManyToOne(const ManyToOne& manyToOne);

		ManyToOne(const QString& name);

		~ManyToOne();

		ManyToOne& operator = (const ManyToOne& manyToOne);

		QString getName() const;

		void addColumnMapping(const ColumnMapping& columnMapping);
		QList<ColumnMapping> getColumnMapingList() const;

		void setClassName(const QString& className);
		QString getClassName() const;

		void setLazy(Lazy lazy);
		Lazy getLazy() const;

		void setCascade(Cascade cascade);
		Cascade getCascade() const;

		bool isValid() const;

	private:

		ManyToOneData* mData;

		void setData(ManyToOneData* data);

	};

}

#endif