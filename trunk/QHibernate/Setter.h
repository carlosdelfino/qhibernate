
#ifndef QHibernate_Setter
#define QHibernate_Setter

#include <QHibernate/StrongPointer.h>

class QObject;
class QVariant;

namespace QHibernate
{
	class Setter
	{
	public:

		virtual ~Setter();

		virtual void set(StrongPointer<QObject>& target, 
			const QVariant& value) = 0;
	};
}

#endif
