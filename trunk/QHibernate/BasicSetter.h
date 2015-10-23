
#ifndef QHibernate_BasicSetter
#define QHibernate_BasicSetter

#include <QHibernate/Setter.h>

#include <QtCore/QString>

class QMetaObject;
class QMetaProperty;

namespace QHibernate
{
	class BasicSetter
		: public Setter
	{
	public:

		BasicSetter(const QMetaObject& metaObject, 
			const QMetaProperty& metaProperty, 
			const QString& propertyName);

		virtual ~BasicSetter();

		virtual void set(StrongPointer<QObject>& target, 
			const QVariant& value);

	private:

		const QMetaObject& mMetaObject; 

		const QMetaProperty& mMetaProperty;

		QString mPropertyName;
	};
}


#endif