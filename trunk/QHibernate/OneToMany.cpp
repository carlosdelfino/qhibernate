
#include <QHibernate/OneToMany.h>

namespace QHibernate
{
	OneToMany::OneToMany()
		: mNotFound(eException)
		, mEmbedXml(true)
	{
	}

	const QString& OneToMany::getClassName() const
	{
		return mClassName;
	}

	void OneToMany::setClassName( const QString& className )
	{
		mClassName = className;
	}

	OneToMany::NotFound OneToMany::getNotFound() const
	{
		return mNotFound;
	}

	void OneToMany::setNotFound(NotFound notFound)
	{
		mNotFound = notFound;
	}

	const QString& OneToMany::getNode() const
	{
		return mNode;
	}

	void OneToMany::setNode( const QString& node )
	{
		mNode = node;
	}

	void OneToMany::setEmbedXml( bool isEmbedXml )
	{
		mEmbedXml = isEmbedXml;
	}

	bool OneToMany::isEmbedXml() const
	{
		return mEmbedXml;
	}

	const QString& OneToMany::getEntityName() const
	{
		return mEntityName;
	}

	void OneToMany::setEntityName( const QString& entityName )
	{
		mEntityName = entityName;
	}

	bool OneToMany::isValid() const
	{
		bool valid = true;
		if (mClassName.isEmpty() && mEntityName.isEmpty())
		{
			valid = false;
		}

		return valid;
	}
}
