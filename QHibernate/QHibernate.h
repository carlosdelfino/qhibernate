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


template <typename T>
struct CustomUserTypeId
{
};

#define QHIBERNATE_CUSTOM_USER_TYPE(componentType, customUserType)	\
template<>															\
struct CustomUserTypeId<##customUserType##>							\
{																	\
	static const QHibernate::CustomUserType* getCustomUserType()	\
	{																\
		static customUserType instance;								\
		return (QHibernate::CustomUserType*)&instance;				\
	}																\
																	\
	static const bool registered;									\
};																	\
																	\
const bool CustomUserTypeId < customUserType >::registered =		\
	QHibernate::CustomUserTypeRegistery::registerCustomUserType(	\
	#componentType,													\
	CustomUserTypeId< customUserType >::getCustomUserType);

#endif // QHibernate_H
