
#ifndef QHibernate_Transaction
#define QHibernate_Transaction

#include <QHibernate/QHibernate_global.h>

namespace QHibernate
{
	class QHIBERNATE_API Transaction
	{
	public:

		void commit();

	private:

		friend class Session;

		Transaction(const Session& session);
	};

}

#endif
