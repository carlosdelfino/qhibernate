
#include <QHibernate/PropertyMapping.h>

namespace QHibernate
{
	PropertyMapping::PropertyMapping()
	{
	}

	void PropertyMapping::setName( const QString& name )
	{
		mName = name;
	}

	const QString& PropertyMapping::getName() const
	{
		return mName;
	}

	void PropertyMapping::setType(const QString& type) 
	{
		mType = type;
	}

	QString PropertyMapping::getType() const
	{
		return mType;
	}

	const QVector<ColumnMapping>& PropertyMapping::getColumnMappings() const
	{
		return mColumnMappings;
	}

	void PropertyMapping::addColumnMapping(const ColumnMapping& columnMapping)
	{
		mColumnMappings.push_back(columnMapping);
	}

	qint32 PropertyMapping::getColumnCount() const
	{
		return mColumnMappings.size();
	}

	ColumnMapping PropertyMapping::getColumn( qint32 index ) const
	{
		return mColumnMappings.at(index);
	}
}
