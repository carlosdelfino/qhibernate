
#ifndef QHibernate_PropertyAccesor
#define QHibernate_PropertyAccesor 

namespace QHibernate
{
	class Setter;
	class Getter;

	class PropertyAccesor
	{
		virtual ~PropertyAccesor();

		virtual const Getter& getGetter() = 0;
		virtual const Setter& getSetter() = 0;
	};
}

#endif
