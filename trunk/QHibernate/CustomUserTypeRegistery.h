
#ifndef QHibernate_CustomUserTypeRegistery
#define QHibernate_CustomUserTypeRegistery

#include <QHibernate/QHibernate_global.h>

#include <QtCore/QHash>
#include <QtCore/QString>

namespace QHibernate
{
	class CustomUserType;

	class QHIBERNATE_API CustomUserTypeRegistery
	{
	public:

		typedef const CustomUserType* (*CsCusmtomUserTypeGetter)();	

		static bool registerCustomUserType(const QString& typeName, 
			CsCusmtomUserTypeGetter getter);

		static const CustomUserType* getCustomUserType(const QString& typeName);

		static bool hasType(const QString& typeName);

	private:

		static CustomUserTypeRegistery& getInstance();

		CustomUserTypeRegistery();

		CustomUserTypeRegistery(const CustomUserTypeRegistery&);

		QHash<QString, CsCusmtomUserTypeGetter> mCustomUserTypeGetters;
	};
}

#endif
