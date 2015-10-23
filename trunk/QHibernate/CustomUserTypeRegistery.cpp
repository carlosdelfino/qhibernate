
#include <QHibernate/CustomUserTypeRegistery.h>
#include <QHibernate/HibernateException.h>

namespace QHibernate
{
	CustomUserTypeRegistery::CustomUserTypeRegistery()
	{
	}

	CustomUserTypeRegistery::CustomUserTypeRegistery(const CustomUserTypeRegistery&)
	{
	}

	CustomUserTypeRegistery& CustomUserTypeRegistery::getInstance()
	{
		static CustomUserTypeRegistery instance;
		return instance;
	}

	bool CustomUserTypeRegistery::registerCustomUserType(const QString& typeName, 
		CsCusmtomUserTypeGetter getter)
	{
		bool registered = false;

		if (0 != getter)
		{
			if (!getInstance().mCustomUserTypeGetters.contains(typeName))
			{
				getInstance().mCustomUserTypeGetters.insert(typeName, getter);
				registered = true;
			}
		}

		return registered;
	}

	const CustomUserType* CustomUserTypeRegistery::getCustomUserType(const QString& typeName)
	{
		if (!getInstance().mCustomUserTypeGetters.contains(typeName))
		{
			throw HibernateException(
				QString("Custom user type is not registered for class(%1)." 
				"Use QHibernate_CUSTOM_USER_TYPE macro to register.").arg(typeName));
		}
		CsCusmtomUserTypeGetter getter = getInstance().mCustomUserTypeGetters.value(typeName);
		return getter();
	}

	bool CustomUserTypeRegistery::hasType( const QString& typeName )
	{
		return getInstance().mCustomUserTypeGetters.contains(typeName);
	}
}
