
#ifndef QHibernate_IdentifierMapping
#define QHibernate_IdentifierMapping

#include <QHibernate/PropertyMapping.h>
#include <QHibernate/GeneratorMapping.h>

namespace QHibernate
{
	class IdentifierMapping
		: public PropertyMapping
	{
	public:

		const GeneratorMapping& getGeneratorMapping() const;

		void setGeneratorMapping(const GeneratorMapping& generatorMapping);

	protected:

	private:

		GeneratorMapping mGeneratorMapping;

	};

}

#endif
