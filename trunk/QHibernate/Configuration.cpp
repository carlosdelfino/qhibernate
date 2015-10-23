
#include <QHibernate/Configuration.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/ConfigurationParser.h>
#include <QHibernate/ConfigurationData.h>
#include <QHibernate/HibernateException.h>

#include <QtCore/QTextStream>
#include <QtCore/QFile>

namespace QHibernate
{
	Configuration::Configuration()
		: mData(0)
	{
		setData(new ConfigurationData());
	}

	Configuration::Configuration(const Configuration& configuration)
		: mData(0)
	{
		*this = configuration;
	}

	Configuration::~Configuration()
	{
		setData(0);
	}

	Configuration& Configuration::operator = (const Configuration& configuration)
	{
		setData(configuration.mData);
		return *this;
	}

	Configuration& Configuration::configure()
	{
		return configure("hibernate.cfg.xml");
	}

	SessionFactory Configuration::buildSessionFactory()
	{
		SessionFactory sessionFactory(*this);

		ConfigurationParser parser;
		parser.parseSessionFactory(sessionFactory, mData->getConfigurationXml());

		return sessionFactory;
	}

	Configuration& Configuration::configure(const QString& path)
	{
		QFile file(path);

		bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
		if (!opened)
		{
			throw HibernateException(
				QString("Could not open configuration file(%1). %2")
				.arg(path)
				.arg(file.errorString()));
		}

		QTextStream textSream(&file);
		return configure(textSream);
	}

	Configuration& Configuration::configure(QTextStream& stream)
	{
		QString xmlContent = stream.readAll();
		mData->setConfigurationXml(xmlContent);
		return *this;
	}

	void Configuration::setData(ConfigurationData* data)
	{
		if (0 != mData)
		{
			mData->removeReference();
			ConfigurationData::destroy(mData);
			mData = 0;
		}

		mData = data;

		if (0 != mData)
		{
			mData->addReference();
		}
	}
}
