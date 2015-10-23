#ifndef QHibernate_Getter
#define QHibernate_Getter

#include <QHibernate/StrongPointer.h>
#include <QtCore/QVariant>

class Session;
class QObject;

namespace QHibernate
{
	class Getter
	{
	public:

		virtual ~Getter();

		virtual QVariant get(StrongPointer<QObject>& object) = 0;

		virtual QVariant getForInsert(StrongPointer<QObject>& object, const Session& session) = 0;

		virtual QMetaProperty getMember() = 0;

		virtual QVariant::Type getReturnType() = 0;
	};
}

#endif
