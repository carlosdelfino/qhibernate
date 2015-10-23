
#include <QHibernate/IdentifierGeneratorFactory.h>
#include <QHibernate/NativeGenerator.h>
#include <QHibernate/ForeignGenerator.h>

namespace QHibernate
{
	IdentifierGeneratorFactory& IdentifierGeneratorFactory::getInstance()
	{
		static IdentifierGeneratorFactory instance;
		return instance;
	}

	IdentifierGenerator* IdentifierGeneratorFactory::getGenerator(const QString& name)
	{
		return mGenerators.value(name);
	}

	IdentifierGeneratorFactory::IdentifierGeneratorFactory()
	{
		mGenerators.insert("native", new NativeGenerator());
		mGenerators.insert("foreign", new ForeignGenerator());
	}

}
