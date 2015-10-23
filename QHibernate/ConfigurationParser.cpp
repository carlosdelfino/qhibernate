
#include <QHibernate/ConfigurationParser.h>
#include <QHibernate/SessionFactory.h>
#include <QHibernate/MappingParser.h>
#include <QHibernate/HibernateException.h>

#include <QtXml/QDomDocument>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QSettings>

namespace QHibernate
{
	void ConfigurationParser::parseSessionFactory(SessionFactory& sessionFactory, 
		const QString& xmlContent)
	{
		QDomDocument domDocument;

		QString errorMessage;
		qint32 errorLine = -1;
		qint32 errorColumn = -1;

		bool result = domDocument.setContent(xmlContent, 
			&errorMessage, 
			&errorLine, 
			&errorColumn);

		if (result)
		{
			QDomElement rootElement = domDocument.documentElement();
			QDomElement sessionFactoryElement = rootElement.firstChildElement("session-factory");

			if (!sessionFactoryElement.isNull())
			{
				QDomElement childElement = sessionFactoryElement.firstChildElement();
				while(!childElement.isNull())
				{
					QString tagName = childElement.tagName();

					if ("property" == tagName)
					{
						QString propertyName = childElement.attribute("name");
						if ("QHibernate.Connection.DriverName" == propertyName)
						{
							QString driverName = applySettings(childElement.text());
							sessionFactory.setDriverName(driverName);
						}
						else if ("QHibernate.Connection.DatabaseName" == propertyName)
						{
							QString databaseName = applySettings(childElement.text());
							sessionFactory.setDatabaseName(databaseName);
						}
						else if ("QHibernate.Connection.HostName" == propertyName)
						{
							QString hostName = applySettings(childElement.text());
							sessionFactory.setHostName(hostName);
						}
						else if ("QHibernate.Connection.UserName" == propertyName)
						{
							QString userName = applySettings(childElement.text());
							sessionFactory.setUserName(userName);
						}
						else if ("QHibernate.Connection.Password" == propertyName)
						{
							QString password = applySettings(childElement.text());
							sessionFactory.setPassword(password);
						}
						else if ("QHibernate.hbm2ddl.auto" == propertyName)
						{

						}
						else if ("QHibernate.hbm2ddl.import_files" == propertyName)
						{

						}
						else
						{
							throw HibernateException(QString("Unknown property(%1)").arg(propertyName));
						}
					}
					else if ("mapping" == tagName)
					{
						QString resource = childElement.attribute("resource");
						MappingParser classMappingParser;					
						classMappingParser.parseMapping(resource, sessionFactory);
					}

					childElement = childElement.nextSiblingElement();
				}
			}
		}
	}

	QString ConfigurationParser::applySettings( const QString& text )
	{
		QString resultText = text;
		int index = 0;
		while((index = resultText.indexOf("${", index)) >= 0)
		{
			int to = resultText.indexOf("}", index);
			QString setting = resultText.mid(index, to - index);

			setting = setting.right(setting.size() - 2);
			QStringList tokens = setting.split("/");

			QSettings settings;
			for (int i = 0; i < tokens.size() -1 ; ++i)
			{
				settings.beginGroup(tokens.at(i));
			}

			setting = settings.value(tokens.at(tokens.size()-1)).toString();

			resultText.replace(index, to - index + 1, setting);
		}

		return resultText;
	}
}
