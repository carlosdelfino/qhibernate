
#ifndef QHibernate_IdentifierGenerator
#define QHibernate_IdentifierGenerator


#include <QHibernate/Pointer.h>

class QObject;
class QSqlRecord;

namespace QHibernate
{
	class Session;
	class GeneratorMapping;
	class IdentifierMapping;

	class IdentifierGenerator
	{
	public:

		virtual ~IdentifierGenerator()
		{
		}

		virtual void serialize(
			const Session& session, 
			const Pointer<QObject>& object,
			QSqlRecord& sqlRecord,
			const IdentifierMapping& idMapping,
			const GeneratorMapping& generatorMapping) = 0;
	};
}

#endif
