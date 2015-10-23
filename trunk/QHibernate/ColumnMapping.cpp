
#include <QHibernate/ColumnMapping.h>

namespace QHibernate
{
	ColumnMapping::ColumnMapping()
		: mLength(0)
	{

	}

	void ColumnMapping::setName( const QString& name )
	{
		mName = name;
	}

	QString ColumnMapping::getName() const
	{
		return mName;
	}

	void ColumnMapping::setLength(qint32 length)
	{
		mLength = length;
	}

	qint32 ColumnMapping::getLength() const
	{
		return mLength;
	}
}
