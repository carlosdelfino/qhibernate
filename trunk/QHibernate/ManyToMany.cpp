
#include <QHibernate/ManyToMany.h>

namespace QHibernate
{
	ManyToMany::ManyToMany()
	{
	}

	bool ManyToMany::isValid() const
	{
		return !mClassName.isEmpty();
	}

	QString ManyToMany::getClassName() const
	{
		return mClassName;
	}

	void ManyToMany::setClassName( const QString& className )
	{
		mClassName = className;
	}

	const QVector<ColumnMapping>& ManyToMany::getColumnMappings() const
	{
		return mColumnMappings;
	}

	void ManyToMany::addColumnMapping( const ColumnMapping& columnMapping )
	{
		mColumnMappings.append(columnMapping);
	}
}
