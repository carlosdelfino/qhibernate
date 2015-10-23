
#include <QHibernate/Class.h>
#include <QHibernate/IdentifierMapping.h>
#include <QHibernate/PropertyList.h>
#include <QHibernate/ManyToOneList.h>
#include <QHibernate/ListMappingList.h>
#include <QHibernate/OneToOneList.h>
#include <QHibernate/QHibernate.h>
#include <QHibernate/ClassList.h>

namespace QHibernate
{
	struct ClassData
	{
	public:

		ClassData(const QString className)
			: mClassName(className)
			, mRefCount(0)
			, mSubclass(false)
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

		QString mClassName;

		QString mTableName;

		QString mExtends;

		bool mSubclass;

		IdentifierMapping mIdentifierMapping;

		PropertyList mPropertyMappings;

		ManyToOneList mManyToOneMappings;

		OneToOneList mOneToOneMappings;

		ListMappingList mListMappingList;

		KeyMapping mKey;

		int mRefCount;

		ClassList mJoinedSubclassList;
	};

	Class::Class()
		: mData(0)
	{

	}

	Class::Class(const QString& className)
		: mData(0)
	{
		setData(new ClassData(className));
	}

	Class::Class(const Class& classMapping )
		: mData(0)
	{
		*this = classMapping;
	}

	Class::~Class()
	{
		setData(0);
	}

	Class& Class::operator=(const Class& classMapping)
	{
		setData(classMapping.mData);
		return *this;
	}

	void Class::setData(ClassData* data)
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

	void Class::setTableName(const QString& tableName)
	{
		if (isValid())
		{
			mData->mTableName = tableName;
		}

	}

	QString Class::getClassName() const
	{
		QString className;
		if (isValid())
		{
			className = mData->mClassName;
		}
		return className;
	}

	QString Class::getTableName() const
	{
		QString tableName = mData->mTableName;
		if (isValid())
		{
			tableName = mData->mTableName;
		}
		return tableName;
	}

	IdentifierMapping Class::getIdentifierMapping() const
	{
		IdentifierMapping idMapping;
		if (isValid())
		{
			idMapping = mData->mIdentifierMapping;
		}
		return idMapping;
	}

	PropertyList Class::getProperties() const
	{
		PropertyList propertyList;

		if (isValid())
		{
			propertyList = mData->mPropertyMappings;
		}

		return propertyList;
	}

	void Class::setIdentifierMapping(const IdentifierMapping& identifierMapping)
	{
		if (isValid())
		{
			mData->mIdentifierMapping = identifierMapping;
		}	
	}

	void Class::addProperty(const PropertyMapping& property)
	{
		if (isValid())
		{
			mData->mPropertyMappings.push_back(property);
		}
	}

	void Class::addManyToOne(const ManyToOne& manyToOne)
	{
		if (isValid())
		{
			mData->mManyToOneMappings.push_back(manyToOne);
		}

	}

	ManyToOneList Class::getManyToOneList() const
	{
		ManyToOneList manyToOneList;

		if (isValid())
		{
			manyToOneList = mData->mManyToOneMappings;
		}

		return manyToOneList;
	}

	void Class::addListMapping( const ListMapping& listMapping )
	{
		if (isValid())
		{
			mData->mListMappingList.push_back(listMapping);
		}	
	}

	ListMappingList Class::getListMappingList() const
	{
		ListMappingList listMappingList;

		if (isValid())
		{
			listMappingList = mData->mListMappingList;
		}

		return listMappingList;
	}

	void Class::addOneToOne(const OneToOne& oneToOne)
	{
		if (isValid())
		{
			mData->mOneToOneMappings.push_back(oneToOne);
		}	
	}

	OneToOneList Class::getOneToOneList() const
	{
		OneToOneList oneToOneList;

		if (isValid())
		{
			oneToOneList = mData->mOneToOneMappings;
		}

		return oneToOneList;
	}

	bool Class::isSubclass() const
	{
		bool result = false;

		if (isValid())
		{
			result = mData->mSubclass;
		}

		return result;
	}

	QString Class::getExtends() const
	{
		QString extends;

		if (isValid())
		{
			extends = mData->mExtends;
		}

		return extends;
	}

	bool Class::isValid() const
	{
		return (0 != mData);
	}

	void Class::setSubclass( bool value )
	{
		if (isValid())
		{
			mData->mSubclass = true;
		}
	}

	void Class::setExtends( const QString& extends )
	{
		if (isValid())
		{
			mData->mExtends = extends;
		}
	}

	void Class::setKey( const KeyMapping& key )
	{
		if (isValid())
		{
			mData->mKey = key;
		}
	}

	KeyMapping Class::getKey() const
	{
		KeyMapping key;

		if (isValid())
		{
			key = mData->mKey;
		}

		return key;
	}

	void Class::addJoinedSubclass(const Class& joinedSubclass)
	{
		if (isValid())
		{
			mData->mJoinedSubclassList.push_back(joinedSubclass);
		}
	}

	bool Class::hasSubclass() const
	{
		bool result = false;

		if (isValid())
		{
			result = !(mData->mJoinedSubclassList.isEmpty());
		}

		return result;
	}

	ClassList Class::getJoinedSubclassList() const
	{
		ClassList joinedSubClassList;

		if (isValid())
		{
			joinedSubClassList = mData->mJoinedSubclassList;
		}

		return joinedSubClassList;
	}

	PropertyMapping Class::getProperty( const QString& propertyName ) const
	{
		PropertyMapping property;

		if (isValid())
		{
			foreach(PropertyMapping currentProperty, mData->mPropertyMappings)
			{
				if (currentProperty.getName() == propertyName)
				{
					property = currentProperty;
					break;
				}
			}
		}

		return property;
	}
}
