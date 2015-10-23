
#ifndef QHibernate_NativeGenerator
#define QHibernate_NativeGenerator

#include <QHibernate/IdentifierGenerator.h>

namespace QHibernate
{
	class NativeGenerator
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
