
#ifndef QHibernate_CsMapping
#define QHibernate_CsMapping

#include <QHibernate/QHibernate_global.h>
#include <QHibernate/PropertyList.h>
#include <QHibernate/ManyToOneList.h>
#include <QHibernate/OneToOneList.h>
#include <QHibernate/ListMappingList.h>
#include <QHibernate/ClassList.h>

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

namespace QHibernate
{
	class ClassData;
	class IdentifierMapping;

	class QHIBERNATE_API Class
	{
	public:

		Class();

		Class(const QString& className);

		Class(const Class& classMapping);

		virtual ~Class();

		Class& operator = (const Class& classMapping);

		//void setClassName(const QString& className);

		void setTableName(const QString& tableName);

		QString getClassName() const;

		QString getTableName() const;

		IdentifierMapping getIdentifierMapping() const;

		void setIdentifierMapping(const IdentifierMapping& identifierMapping);

		PropertyList getProperties() const;

		void addProperty(const PropertyMapping& property);

		ManyToOneList getManyToOneList() const;

		void addManyToOne(const ManyToOne& manyToOne);

		void addOneToOne(const OneToOne& oneToOne);

		OneToOneList getOneToOneList() const;

		void addListMapping(const ListMapping& listMapping);

		ListMappingList getListMappingList() const;

		bool isSubclass() const;

		QString getExtends() const;

		bool isValid() const;

		void setSubclass( bool value );

		void setExtends( const QString& extends );

		void setKey(const KeyMapping& key);

		KeyMapping getKey() const;

		void addJoinedSubclass(const Class& joinedSubclass);

		ClassList getJoinedSubclassList() const;

		bool hasSubclass() const;

		PropertyMapping getProperty( const QString& propertyName ) const;

	private:

		ClassData* mData;

		void setData(ClassData* data);
	};
}

#endif
