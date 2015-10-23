
#ifndef QHibernate_MetaObjectRegistery
#define QHibernate_MetaObjectRegistery

#include <QHibernate/QHibernate_global.h>

#include <QtCore/QHash>
#include <QtCore/QString>

class QMetaObject;

namespace QHibernate
{
	class QHIBERNATE_API MetaObjectRegistery
	{
	public:

		typedef const QMetaObject& (*CsMetaObjectGetter)();

		static MetaObjectRegistery& getInstance();

		bool registerClass(const QString& className, CsMetaObjectGetter getter);

		const QMetaObject& getMetaObject(const QString& className) const;

	private:

		MetaObjectRegistery();

		QHash<QString, CsMetaObjectGetter> mMetaObjectGetters;
	};

}

#endif
