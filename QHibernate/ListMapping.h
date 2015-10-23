
#ifndef QHibernate_CsListProperty
#define QHibernate_CsListProperty

#include <QtCore/QString>

#include <QHibernate/KeyMapping.h>
#include <QHibernate/OneToMany.h>
#include <QHibernate/ManyToMany.h>

namespace QHibernate
{
	class ListMapping
	{
	public:

		ListMapping();

		void setLazy(bool isLazy);
		bool isLazy() const;

		void setName(const QString& name);
		QString getName() const;

		void setTable(const QString& table);
		QString getTable() const;

		void setOneToManyMapping(const OneToMany& oneToManyMapping);
		const OneToMany& getOneToMany() const;

		void setKeyMapping(const KeyMapping& keyMapping);
		const KeyMapping& getKeyMapping() const;

		bool isNull() const;

		void setManyToMany(const ManyToMany& manyToMany);
		const ManyToMany& getManyToMany() const;

	private:

		QString mName;

		QString mTable;

		KeyMapping mKeyMapping;

		OneToMany mOneToManyMapping;

		ManyToMany mManyToMany;

		bool mLazy;
	};

}

#endif