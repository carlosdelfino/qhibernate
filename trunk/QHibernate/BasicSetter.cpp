
#include <QHibernate/BasicSetter.h>
#include <QHibernate/HibernateException.h>

#include <QtCore/QMetaProperty>

namespace QHibernate
{
	BasicSetter::BasicSetter(const QMetaObject& metaObject, 
		const QMetaProperty& metaProperty, 
		const QString& propertyName)
		: mMetaObject(metaObject)
		, mMetaProperty(metaProperty)
		, mPropertyName(propertyName)
	{

	}

	BasicSetter::~BasicSetter()
	{

	}

	void BasicSetter::set(StrongPointer<QObject>& target, const QVariant& value)
	{
		if (target.isNull())
		{
			throw HibernateException("Could not set property. Entity is null.");
		}

		if (!mMetaProperty.isWritable())
		{
			throw HibernateException("Could not set property. Property is read only.");
		}

		bool success = mMetaProperty.write(target.getPointer(), value);

		if (!success)
		{
			throw HibernateException("Could not set property.");
		}
	}
}
