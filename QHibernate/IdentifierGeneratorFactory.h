
#ifndef QHibernate_IdentifierGeneratorFactory
#define QHibernate_IdentifierGeneratorFactory

#include <QtCore/QHash>
#include <QtCore/QString>

#include <QHibernate/StrongPointer.h>

namespace QHibernate
{
	class IdentifierGenerator;

	class IdentifierGeneratorFactory
	{
	public:

		static IdentifierGeneratorFactory& getInstance();

		IdentifierGenerator* getGenerator(const QString& name);

	private:

		IdentifierGeneratorFactory();

		IdentifierGeneratorFactory(const IdentifierGeneratorFactory&);

		IdentifierGeneratorFactory& operator = (const IdentifierGeneratorFactory&);

		QHash<QString, IdentifierGenerator*> mGenerators;

	};

}


#endif
