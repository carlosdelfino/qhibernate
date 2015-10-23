
#include <QHibernate\OneToOne.h>
#include <QHibernate\QHibernate.h>

namespace QHibernate
{
	class OneToOneData
	{
	public:

		OneToOneData(const QString& name)
			: mName(name)
			, mOuterJoin(eOuterJoinAuto)
			, mFetch(eFecthSelect)
			, mConstrainted(false)
			, mLazy(eLazyProxy)
			, mEmbedXml(true)
			, mCascade(eCascadeNone)
			, mRefCount(0)
		{
		}

		QString mName;
		QString mFormula;
		QString mAccess;
		QString mClassName;
		QString mEntityName;
		Cascade mCascade;
		OuterJoin mOuterJoin;
		Fetch mFetch;
		bool mConstrainted;
		QString mForeignKey;
		QString mPropertyRef;
		Lazy mLazy;
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

	OneToOne::OneToOne()
		: mData(0)
	{
	}

	OneToOne::OneToOne(const QString& name)
		: mData(0)
	{
		setData(new OneToOneData(name));
	}

	OneToOne::OneToOne(const OneToOne& oneToOne)
		: mData(0)
	{
		*this = oneToOne;
	}

	OneToOne::~OneToOne()
	{
		setData(0);
	}

	QString OneToOne::getName() const
	{
		QString name;

		if (isValid())
		{
			name = mData->mName;
		}

		return name;
	}

	void OneToOne::setClassName( const QString& className )
	{
		if (isValid())
		{
			mData->mClassName = className;
		}
	}

	QString OneToOne::getClassName() const
	{
		QString className;
		if (isValid())
		{
			className = mData->mClassName;
		}
		return className;
	}

	void OneToOne::setCascade(Cascade cascade)
	{
		if (isValid())
		{
			mData->mCascade = cascade;
		}
	}

	Cascade OneToOne::getCascade() const
	{
		Cascade cascade = eCascadeNone;
		if (isValid())
		{
			cascade = mData->mCascade;
		}
		return cascade;
	}

	void OneToOne::setLazy(Lazy lazy)
	{
		if (isValid())
		{
			mData->mLazy = lazy;
		}
	}

	Lazy OneToOne::getLazy() const
	{
		Lazy lazy = eLazyFalse;
		if (isValid())
		{
			lazy = mData->mLazy;
		}
		return lazy;
	}

	void OneToOne::setData(OneToOneData* data)
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

	bool OneToOne::isValid() const
	{
		return (0 != mData);
	}

	OneToOne& OneToOne::operator=(const OneToOne& oneToOne)
	{
		setData(oneToOne.mData);
		return *this;
	}
}
