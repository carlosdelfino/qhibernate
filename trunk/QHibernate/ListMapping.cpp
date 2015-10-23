
#include <QHibernate/ListMapping.h>

namespace QHibernate
{
	ListMapping::ListMapping()
		:mLazy(true)
	{
	}

	void ListMapping::setName(const QString& name)
	{
		mName = name;
	}

	QString ListMapping::getName() const
	{
		return mName;
	}

	void ListMapping::setTable(const QString& table)
	{
		mTable = table;
	}

	QString ListMapping::getTable() const
	{
		return mTable;
	}

	void ListMapping::setKeyMapping( const KeyMapping& keyMapping )
	{
		mKeyMapping = keyMapping;
	}

	const KeyMapping& ListMapping::getKeyMapping() const
	{
		return mKeyMapping;
	}

	void ListMapping::setOneToManyMapping( const OneToMany& oneToManyMapping )
	{
		mOneToManyMapping = oneToManyMapping;
	}

	const OneToMany& ListMapping::getOneToMany() const
	{
		return mOneToManyMapping;
	}

	void ListMapping::setLazy( bool isLazy )
	{
		mLazy = isLazy;
	}

	bool ListMapping::isLazy() const
	{
		return mLazy;
	}

	bool ListMapping::isNull() const
	{
		return mName.isEmpty();
	}

	void ListMapping::setManyToMany(const ManyToMany& manyToMany)
	{
		mManyToMany = manyToMany;
	}

	const ManyToMany& ListMapping::getManyToMany() const
	{
		return mManyToMany;
	}
}
