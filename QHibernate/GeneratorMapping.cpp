
#include <QHibernate/GeneratorMapping.h>

namespace QHibernate
{
	void GeneratorMapping::addParameter(const QString& name, const QString& value)
	{
		mParameters.insert(name, value);
	}

	QString GeneratorMapping::getName() const
	{
		return mName;
	}

	void GeneratorMapping::setName(const QString& name) 
	{
		mName = name;
	}

	QString GeneratorMapping::getParamValue( const QString& paramName ) const
	{
		return mParameters.value(paramName);
	}
}

