
#ifndef QHibernate_Cascade
#define QHibernate_Cascade

namespace QHibernate
{
	enum Cascade
	{
		eCascadeNone		= 0,
		eCascadeSaveUpdate	= 1,
		eCascadeDelete		= 2,
		eCascadeAll = eCascadeSaveUpdate | eCascadeDelete,
	};

}

#endif
