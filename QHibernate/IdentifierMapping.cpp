
#include <QHibernate/IdentifierMapping.h>

namespace QHibernate
{
	const GeneratorMapping& IdentifierMapping::getGeneratorMapping() const
	{
		return mGeneratorMapping;
	}

	void IdentifierMapping::setGeneratorMapping( const GeneratorMapping& generatorMapping )
	{
		mGeneratorMapping = generatorMapping;
	}
}

