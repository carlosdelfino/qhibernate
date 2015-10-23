
#include <QHibernate\ManyToOne.h>
#include <QHibernate\QHibernate.h>

namespace QHibernate
{
	struct ManyToOneData
	{
	public:

		ManyToOneData(const QString& name)
			: mName(name)
			, mNotNull(false)
			, mUnique(false)
			, mCascade(eCascadeNone)
			, mOuterJoin(eOuterJoinAuto)
			, mFetch(eFecthSelect)
			, mUpdate(true)
			, mInsert(true)
			, mOptimisticLock(true)
			, mLazy(eLazyProxy)
			, mNotFound(eNotFoundException)
			, mEmbedXml(true)
			, mRefCount(0)
		{
		}

		QString mName;

		QString mAccess;

		QString mClassName;

		QString mEntityName;

		QList<ColumnMapping> mColumnMappingList;

		bool mNotNull;

		bool mUnique;

		QString mUniqueKey;

		QString mIndex;

		Cascade mCascade;

		OuterJoin mOuterJoin;

		Fetch mFetch;

		bool mUpdate;

		bool mInsert;

		bool mOptimisticLock;

		QString mForeignKey;

		QString mPropertyRef;

		QString mFormula;

		Lazy mLazy;

		NotFound mNotFound;

		QString mNode;

		bool mEmbedXml;

		int mRefCount;

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
	};

	ManyToOne::ManyToOne()
		: mData(0)
	{
	}

	ManyToOne::ManyToOne(const ManyToOne& manyToOne)
		: mData(0)
	{
		*this = manyToOne;
	}

	ManyToOne::ManyToOne(const QString& name)
		: mData(0)
	{
		setData(new ManyToOneData(name));
	}


	ManyToOne::~ManyToOne()
	{
		setData(0);
	}

	ManyToOne& ManyToOne::operator=(const ManyToOne& manyToOne)
	{
		setData(manyToOne.mData);
		return *this;
	}

	QString ManyToOne::getName() const
	{
		QString name;
		if (isValid())
		{
			name = mData->mName;
		}
		return name;
	}

	void ManyToOne::setClassName( const QString& className )
	{
		if (isValid())
		{
			mData->mClassName = className;
		}
	}

	QString ManyToOne::getClassName() const
	{
		QString className;

		if (isValid())
		{
			className = mData->mClassName;
		}
		return className;
	}

	void ManyToOne::setLazy( Lazy lazy )
	{
		if (isValid())
		{
			mData->mLazy = lazy;
		}
	}

	Lazy ManyToOne::getLazy() const
	{
		Lazy lazy = eLazyFalse;
		if (isValid())
		{
			lazy = mData->mLazy;
		}
		return lazy;
	}

	void ManyToOne::addColumnMapping( const ColumnMapping& columnMapping )
	{
		if (isValid())
		{
			mData->mColumnMappingList.push_back(columnMapping);
		}
	}

	QList<ColumnMapping> ManyToOne::getColumnMapingList() const
	{
		QList<ColumnMapping> columnMappings;
		if (isValid())
		{
			columnMappings = mData->mColumnMappingList;
		}
		return columnMappings;
	}

	void ManyToOne::setCascade(Cascade cascade)
	{
		if (isValid())
		{
			mData->mCascade = cascade;
		}
	}

	Cascade ManyToOne::getCascade() const
	{
		Cascade cascade = eCascadeNone;

		if (isValid())
		{
			cascade = mData->mCascade;
		}

		return cascade;
	}

	bool ManyToOne::isValid() const
	{
		return (0 != mData);
	}

	void ManyToOne::setData(ManyToOneData* data)
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
}


