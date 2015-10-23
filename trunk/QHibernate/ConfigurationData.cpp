
#include <QHibernate/ConfigurationData.h>

namespace QHibernate
{
	QString ConfigurationData::getConfigurationXml() const
	{
		return mConfigurationXml;
	}

	void ConfigurationData::setConfigurationXml(const QString& configurationXml)
	{
		mConfigurationXml = configurationXml;
	}

}
