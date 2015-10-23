
#ifndef QHibernate_GeneratorMapping
#define QHibernate_GeneratorMapping

#include <QtCore/QString>
#include <QtCore/QHash>

namespace QHibernate
{
	class GeneratorMapping
	{
	public:	

		void addParameter(const QString& name, const QString& value);	

		QString getName() const;

		void setName(const QString& name);

		QString getParamValue(const QString& paramName) const;

	private:

		QString mName;

		QHash<QString, QString> mParameters;
	};
}

#endif
