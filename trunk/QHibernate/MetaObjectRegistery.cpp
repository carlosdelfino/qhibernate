
#include <QHibernate/MetaObjectRegistery.h>
#include <QHibernate/HibernateException.h>

#include <QtCore/QMetaObject>

namespace QHibernate
{
	MetaObjectRegistery::MetaObjectRegistery()
	{

	}

	MetaObjectRegistery& MetaObjectRegistery::getInstance()
	{
		static MetaObjectRegistery instance;
		return instance;
	}

	bool MetaObjectRegistery::registerClass(const QString& className, 
		CsMetaObjectGetter getter)
	{
		bool registered = false;

		if (!mMetaObjectGetters.contains(className))
		{
			mMetaObjectGetters.insert(className, getter);
			registered = true;
		}

		return registered;
	}

	const QMetaObject& MetaObjectRegistery::getMetaObject(const QString& className) const
	{
		if (!mMetaObjectGetters.contains(className))
		{
			throw HibernateException(
				QString("MetaObject is not registered for class(%1)." 
				" Use QHibernate_DATA_CLASS macro to register.").arg(className));
		}
		CsMetaObjectGetter getter = mMetaObjectGetters.value(className);
		return getter();
	}

}
