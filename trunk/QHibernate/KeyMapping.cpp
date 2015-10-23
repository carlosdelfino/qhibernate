
#include <QHibernate\KeyMapping.h>
#include <QHibernate\QHibernate.h>

namespace QHibernate
{
	struct CsKeyData
	{
	public:

		CsKeyData()
			: mNotNull(false)
			, mUpdate(false)
			, mUnique(false)
			, mRefCount(0)
		{

		}

		void addReference()
		{
			mRefCount++;
		}

		void removeReference()
		{
			mRefCount--;
		}

		int getRefCount() const
		{
			return mRefCount;
		}

		ColumnList mColumnList;

		bool mNotNull;

		bool mUpdate;

		bool mUnique;

		QString mPropertyRef;

		QString mForeignKey;

		int mRefCount;
	};

	KeyMapping::KeyMapping()
		: mData(0)
	{
		setData(new CsKeyData());
	}

	KeyMapping::KeyMapping(const KeyMapping& key)
		: mData(0)
	{
		*this = key;
	}

	KeyMapping::~KeyMapping()
	{
		setData(0);
	}

	KeyMapping& KeyMapping::operator=( const KeyMapping& key )
	{
		setData(key.mData);
		return *this;
	}

	void KeyMapping::setData( CsKeyData* data )
	{
		if (0 != mData)
		{
			mData->removeReference();
			if (0 == mData->getRefCount())
			{
				delete mData;
			}
			mData = 0;
		}

		mData = data;

		if (0 != mData)
		{
			mData->addReference();
		}
	}

	ColumnList KeyMapping::getColumnList() const
	{
		ColumnList columnList;

		if (isValid())
		{
			columnList = mData->mColumnList;
		}
		return columnList;
	}

	void KeyMapping::addColumn(const ColumnMapping& column)
	{
		if (isValid())
		{
			mData->mColumnList.push_back(column);
		}

	}

	qint32 KeyMapping::getColumnCount() const
	{
		qint32 columnCount = 0;

		if (isValid())
		{
			columnCount = mData->mColumnList.size();
		}

		return columnCount;	
	}

	ColumnMapping KeyMapping::getColumn(qint32 index) const
	{
		ColumnMapping column;

		if (isValid())
		{
			column = mData->mColumnList.at(index);
		}

		return column;
	}

	bool KeyMapping::isValid() const
	{
		return (0 != mData);
	}
}
