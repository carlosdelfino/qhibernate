
#ifndef QHibernate_SetMapping
#define QHibernate_SetMapping

#include <QHibernate/KeyMapping.h>
#include <QHibernate/Fetch.h>
#include <QHibernate/ManyToMany.h>

namespace QHibernate
{
	class SetMapping
	{
	public:

		SetMapping();

		void setName(const QString& mTable);
		QString getName() const;

		void setTable(const QString& table);
		QString getTable() const;

		void setLazy(bool isLazy);
		bool isLazy();

		void setInverse(bool isInverse);
		bool isInverse() const;

		void setFecth();
		Fetch getFetch() const;

		void setKey(const KeyMapping& keyMapping);
		const KeyMapping& getKeyMapping() const;

		void setMany(const ManyToMany& manyToMany);
		const ManyToMany& getManyToMany() const;
	private:

		QString mName;

		QString mTable;

		bool mLazy;

		bool mInverse;

		Fetch mFetch;

		KeyMapping mKeyMapping;

		ManyToMany mManyToMany;
	};
}

#endif
