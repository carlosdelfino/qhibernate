
#ifndef QHibernate_ForeignGenerator
#define QHibernate_ForeignGenerator

#include <QHibernate/IdentifierGenerator.h>

namespace QHibernate
{
	class ForeignGenerator
		: public IdentifierGenerator
	{
	public:

		virtual void serialize(
			const Session& session, 
			const Pointer<QObject>& object,
			QSqlRecord& sqlRecord,
			const IdentifierMapping& idMapping,
			const GeneratorMapping& generatorMapping);
	};

}

#endif
