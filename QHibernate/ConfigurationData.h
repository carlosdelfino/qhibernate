
#ifndef QHibernate_ConfigurationData
#define QHibernate_ConfigurationData

#include <QHibernate\ReferenceCounter.h>
#include <QtCore\QString>

namespace QHibernate
{
	class ConfigurationData
		: public ReferenceCounter<ConfigurationData>
	{
	public:

		void setConfigurationXml(const QString& configurationXml);

		QString getConfigurationXml() const;

	private:

		QString mConfigurationXml;
	};

}



#endif
