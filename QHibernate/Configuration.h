
#ifndef QHibernate_Configuration
#define QHibernate_Configuration

#include <QHibernate/QHibernate_global.h>

#include <QtCore\QObject>
#include <QtCore\QMap>
#include <QtCore\QString>
#include <QtCore\qsharedpointer.h>

class QTextStream;

namespace QHibernate
{
	class SessionFactory;
	class ConfigurationData;

	class QHIBERNATE_API Configuration
		: public QObject
	{
	public:

		Configuration();

		Configuration(const Configuration& configuration);

		~Configuration();

		Configuration& operator = (const Configuration& configuration);

		Configuration& configure();

		Configuration& configure(const QString& path);

		Configuration& configure(QTextStream& stream);

		SessionFactory buildSessionFactory();

	private:

		ConfigurationData* mData;

		void setData(ConfigurationData* data);
	};
}

#endif