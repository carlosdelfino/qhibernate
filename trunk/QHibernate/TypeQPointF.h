
#ifndef QHibernate_TypeQPointF
#define QHibernate_TypeQPointF

#include <QHibernate/CustomUserType.h>

namespace QHibernate
{
	class TypeQPointF
		: CustomUserType
	{
	public:

		TypeQPointF();

		virtual ~TypeQPointF();

		virtual void setPropertyValue(QVariant& component, 
			qint32 propertyIndex, 
			const QVariant& propertyValue ) const;

		virtual QVariant getPropertyValue(const QVariant& component,
			qint32 propertyIndex) const; 

	protected:


	private:

	};

}

#endif
