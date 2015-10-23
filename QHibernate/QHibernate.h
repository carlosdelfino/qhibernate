#ifndef QHibernate_H
#define QHibernate_H

#include <QHibernate/MetaObjectRegistery.h>
#include <QHibernate/CustomUserTypeRegistery.h>
#include <QHibernate/StrongPointer.h>
#include <QHibernate/WeakPointer.h>
#include <QtCore/QMetaType>


#define QHIBERNATE_DATA_CLASS(moduleName, className) \
	const QMetaObject& get##moduleName##className##MetaObject() \
	{ \
		return moduleName##::##className##::staticMetaObject; \
	} \
	static const bool registered##moduleName##className##1 = \
	MetaObjectRegistery::getInstance().registerClass(#moduleName"::"#className, get##moduleName##className##MetaObject); \
	static const int metaId##moduleName##className## = \
		qRegisterMetaType<StrongPointer<##moduleName##::##className##>>("StrongPointer<"#moduleName"::"#className">"); \
	static const int metaId##moduleName##className##2 = \
		qRegisterMetaType<WeakPointer<##moduleName##::##className##>>("WeakPointer<"#moduleName"::"#className">");

#define QHIBERNATE_CUSTOM_USER_TYPE(componentType, customUserType)

// #define QHIBERNATE_CUSTOM_USER_TYPE(componentType, customUserType) \
// 	const CustomUserType* get##customUserType##() \
// 	{ \
// 		static customUserType instance; \
// 		return (CustomUserType*)&instance; \
// 	} \
// 	static const bool registered##customUserType## = \
// 	CustomUserTypeRegistery::registerCustomUserType(#componentType, get##customUserType##);
// 	

#endif // QHibernate_H
