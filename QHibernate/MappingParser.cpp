
#include <QHibernate\MappingParser.h>
#include <QHibernate\Class.h>
#include <QHibernate\HibernateException.h>
#include <QHibernate\ManyToOne.h>
#include <QHibernate\OneToOne.h>
#include <QHibernate\SessionFactory.h>
#include <QHibernate\ListMapping.h>
#include <QHibernate\OneToMany.h>
#include <QHibernate\KeyMapping.h>
#include <QHibernate\GeneratorMapping.h>
#include <QHibernate\ManyToMany.h>
#include <QHibernate\IdentifierMapping.h>

#include <QtXml/QDomDocument>
#include <QtCore/QFile>

namespace QHibernate
{
	MappingParser::MappingParser()
	{
	}

	void MappingParser::parseMapping(const QString& filePath, 
		SessionFactory& sessionFactory)
	{
		QFile file(filePath);
		bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);

		if (opened)
		{
			try
			{
				parseMapping(&file, sessionFactory);
			}
			catch(HibernateException& exception)
			{
				throw HibernateException(QString("Error while parsing mapping file(%1). %2")
					.arg(filePath)
					.arg(exception.getMessage()));
			}
		}
		else
		{
			throw HibernateException(
				QString("Could not open hibernate mapping file(%1). ")
				.arg(filePath) + file.errorString());
		}
	}

	void MappingParser::parseMapping(QIODevice* device, 
		SessionFactory& sessionFactory)
	{
		QDomDocument domDocument;

		QString errorMessage;
		qint32 errorLine = -1;
		qint32 errorColumn = -1;

		bool parsed = domDocument.setContent(device, &errorMessage, &errorLine, &errorColumn);
		if (parsed)
		{
			QDomElement rootElement = domDocument.documentElement();
			QString tagName = rootElement.tagName();
			if ("hibernate-mapping" == tagName)
			{
				QDomElement childElement = rootElement.firstChildElement();
				while (!childElement.isNull())
				{
					if ("class" == childElement.tagName())
					{
						Class classMapping;
						parseClassMapping(classMapping, childElement);
						sessionFactory.addClassMapping(classMapping);
					}
					else if ("joined-subclass" == childElement.tagName())
					{
						QString extends = childElement.attribute("extends");
						Class extendedClass = sessionFactory.getClassMapping(extends);

						Class classMapping;					
						parseClassMapping(classMapping, childElement);
						classMapping.setSubclass(true);					
						classMapping.setExtends(extends);

						sessionFactory.addClassMapping(classMapping);
						extendedClass.addJoinedSubclass(classMapping);
					}
					else
					{
						throw HibernateException(QString("Unexpected tag (%1).").arg(tagName));
					}

					childElement = childElement.nextSiblingElement();
				}
			}
			else
			{
				throw HibernateException(QString("Unexpected tag (%1).").arg(tagName));
			}
		}
		else
		{
			throw HibernateException(QString("Error at line %1 column %2 in hibernate configuration xml. %3")
				.arg(errorLine)
				.arg(errorColumn)
				.arg(errorMessage));
		}
	}

	void MappingParser::parseIdentifierMapping(IdentifierMapping& identifierMapping, 
		const QDomElement& element)
	{
		QString name = element.attribute("name");
		identifierMapping.setName(name);

		QString type = element.attribute("type");
		identifierMapping.setType(type);

		if (element.hasAttribute("column"))
		{
			QString columnName = element.attribute("column");
			ColumnMapping columnMapping;
			columnMapping.setName(columnName);
			identifierMapping.addColumnMapping(columnMapping);
		}

		QDomElement childElement = element.firstChildElement();
		while (!childElement.isNull())
		{
			QString tagName = childElement.tagName();
			if ("column" == tagName)
			{
				ColumnMapping columnMapping;
				parseColumnMapping(columnMapping, childElement);
				identifierMapping.addColumnMapping(columnMapping);
			}
			else if ("generator" == tagName)
			{
				GeneratorMapping generatorMapping;
				parseGeneratorMapping(generatorMapping, childElement);
				identifierMapping.setGeneratorMapping(generatorMapping);
			}
			else
			{		
			}

			childElement = childElement.nextSiblingElement();
		}
	
	}

	void MappingParser::parseGeneratorMapping(GeneratorMapping& generatorMapping, 
		const QDomElement& element)
	{
		QString name = element.attribute("class");
		generatorMapping.setName(name);

		QDomElement childElement = element.firstChildElement();
		while (!childElement.isNull())
		{
			QString paramName = childElement.attribute("name");
			QString paramValue = childElement.text();
			generatorMapping.addParameter(paramName, paramValue);
			childElement = childElement.nextSiblingElement();
		}
	}

	void MappingParser::parseColumnMapping(ColumnMapping& columnMapping, 
		const QDomElement& element)
	{
		QString name = element.attribute("name");
		columnMapping.setName(name);

		if (element.hasAttribute("length"))
		{
			qint32 length = element.attribute("length").toInt();
			columnMapping.setLength(length);
		}
	}

	void MappingParser::parseProperty(PropertyMapping& property, 
		const QDomElement& element)
	{
		if (!element.hasAttribute("name"))
		{
			throw HibernateException(
				QString("'name' attribute expected for 'property' tag at line %1 column %2.")
					.arg(element.lineNumber())
					.arg(element.columnNumber()));
		}

		QString name = element.attribute("name");
		property.setName(name);

		QString type = element.attribute("type");
		property.setType(type);

		QString column = element.attribute("column");
		if (!column.isEmpty())
		{
			ColumnMapping columnMapping;
			columnMapping.setName(column);
			property.addColumnMapping(columnMapping);
		}
		else
		{
			QDomElement childElement = element.firstChildElement();
			while (!childElement.isNull())
			{
				QString tagName = childElement.tagName();
				if ("column" == tagName)
				{
					ColumnMapping columnMapping;
					parseColumnMapping(columnMapping, childElement);
					property.addColumnMapping(columnMapping);
				}
				else
				{
					throw HibernateException(QString("Unexpected tag (%1)")
						.arg(tagName));
				}

				childElement = childElement.nextSiblingElement();
			}
		}

		if (property.getColumnMappings().isEmpty())
		{
			ColumnMapping columnMapping;
			columnMapping.setName(name);
			property.addColumnMapping(columnMapping);
		}
	}

	void MappingParser::parseClassMapping(Class& classMapping, 
		const QDomElement& element)
	{
		QString className = element.attribute("name");
		classMapping = Class(className);

		QString tableName = element.attribute("table");
		classMapping.setTableName(tableName);

		QDomElement childElement = element.firstChildElement();
		while(!childElement.isNull())
		{
			QString tagName = childElement.tagName();
			if ("id" == tagName)
			{
				IdentifierMapping identifierMapping;
				parseIdentifierMapping(identifierMapping, childElement);
				classMapping.setIdentifierMapping(identifierMapping);						
			}
			else if ("key" == tagName)
			{
				KeyMapping key;
				parseKeyMapping(key, childElement);
				classMapping.setKey(key);
			}
			else if ("property" == tagName)
			{
				PropertyMapping property;
				parseProperty(property, childElement);
				classMapping.addProperty(property);
			}
			else if ("many-to-one" == tagName)
			{
				ManyToOne manyToOne;
				parseManyToOne(manyToOne, childElement);
				classMapping.addManyToOne(manyToOne);
			}
			else if ("one-to-one" == tagName)
			{
				OneToOne oneToOne;
				parseOneToOne(oneToOne, childElement);
				classMapping.addOneToOne(oneToOne);
			}
			else if ("list" == tagName)
			{
				ListMapping listMapping;
				parseListMapping(listMapping, childElement);
				classMapping.addListMapping(listMapping);
			}
	// 		else if ("joined-subclass")
	// 		{
	// 			JoinedSubclass joinedSubclass;
	// 			parseJoinedSubclass(joinedSubclass, childElement);
	// 			joinedSubclass.setExtents(className);
	// 			classMapping.addJoinedSubclass(joinedSubclass);
	// 				
	// 		}
			else
			{
				throw HibernateException(QString("Unexpected tag(%1).").arg(tagName));
			}
			childElement = childElement.nextSiblingElement();
		}	
	}

	bool parseLazy(const QDomElement& element, Lazy& lazy)
	{
		bool result = false;

		if (element.hasAttribute("lazy"))
		{
			QString attributeValue = element.attribute("lazy");
			if ("false" == attributeValue)
			{
				lazy = eLazyFalse;
			}
			else if ("proxy" == attributeValue)
			{
				lazy = eLazyProxy;
			}
			else if ("no-proxy" == attributeValue)
			{
				lazy = eLazyNoproxy;
			}
			else
			{
				throw HibernateException(
					QString("Invalid value(%1) for 'lazy' attribute." 
					"Available values are 'false'|'proxy'|'no-proxy'.").
					arg(attributeValue));
			}

			result = true;
		}

		return result;
	}

	bool parseCascade(const QDomElement& element, Cascade& cascade)
	{
		bool parsed = false;

		if (element.hasAttribute("cascade"))
		{
			QString value = element.attribute("cascade");
			if ("save-update" == value)
			{
				cascade = eCascadeSaveUpdate;
			}
			else if ("delete" == value)
			{
				cascade = eCascadeDelete;
			}
			else if ("all" == value)
			{
				cascade = eCascadeAll;
			}
			else
			{
				throw HibernateException("Invalid value for 'cascade'.");
			}

			parsed = true;
		}

		return parsed;
	}

	void MappingParser::parseManyToOne(ManyToOne& manyToOne, 
		const QDomElement& element)
	{
		if (element.hasAttribute("name"))
		{
			QString name = element.attribute("name");
			manyToOne = ManyToOne(name);

			if (element.hasAttribute("column"))
			{
				QString columnName = element.attribute("column");
				ColumnMapping columnMapping;
				columnMapping.setName(columnName);
				manyToOne.addColumnMapping(columnMapping);
			}

			QString className = element.attribute("class");
			manyToOne.setClassName(className);

			Lazy lazy = eLazyFalse;
			if (parseLazy(element, lazy))
			{
				manyToOne.setLazy(lazy);
			}

			Cascade cascade = eCascadeNone;
			if (parseCascade(element, cascade))
			{
				manyToOne.setCascade(cascade);
			}

			QDomElement childElement = element.firstChildElement();
			while (!childElement.isNull())
			{
				QString tagName = childElement.tagName();
				if ("column" == tagName)
				{
					ColumnMapping columnMapping;
					parseColumnMapping(columnMapping, childElement);
					manyToOne.addColumnMapping(columnMapping);
				}
				else if ("meta" == tagName)
				{
					//ignoring meta tag
				}
				else if ("formula" == tagName)
				{

				}
				else
				{
					throw HibernateException(
						QString("Unexpected tag(%1) for 'many-to-one'.")
							.arg(tagName));
				}
				childElement = childElement.nextSiblingElement();
			}
		}
		else
		{
			throw HibernateException("'name' attribute is mandatory for 'many-to-one' tag.");
		}
	}

	void MappingParser::parseOneToOne(OneToOne& oneToOne, 
		const QDomElement& element)
	{
		if (element.hasAttribute("name"))
		{
			QString name = element.attribute("name");
			oneToOne = OneToOne(name);

			QString className = element.attribute("class");
			oneToOne.setClassName(className);
		
			Lazy lazy = eLazyProxy;
			if (parseLazy(element, lazy))
			{
				oneToOne.setLazy(lazy);
			}

			Cascade cascade = eCascadeNone;
			if (parseCascade(element, cascade))
			{
				oneToOne.setCascade(cascade);
			}
		}
		else
		{
			throw HibernateException(QString("'name' attribute is mandatory for 'one-to-one' tag."));
		}
	}

	// void MappingParser::parseRelationship(Relationship& relationship, 
	// 	const QDomElement& element)
	// {
	// 	parseProperty(relationship, element);
	// 
	// 	QString className = element.attribute("class");
	// 	relationship.setClassName(className);
	// }

	void MappingParser::parseListMapping(ListMapping& listMapping, 
		const QDomElement& element)
	{
		if (element.hasAttribute("name"))
		{
			QString name = element.attribute("name");
			listMapping.setName(name);

			QString tableName = name;
			if (element.hasAttribute("table"))
			{
				tableName = element.attribute("table");
			}
			listMapping.setTable(tableName);

			QDomElement childElement = element.firstChildElement();
			while (!childElement.isNull())
			{
				QString tagName = childElement.tagName();
				if ("key" == tagName)
				{
					KeyMapping keyMapping;
					parseKeyMapping(keyMapping, childElement);
					listMapping.setKeyMapping(keyMapping);
				}
				else if ("one-to-many" == tagName)
				{
					OneToMany oneToManyMapping;
					parseOneToManyMapping(oneToManyMapping, childElement);
					listMapping.setOneToManyMapping(oneToManyMapping);
				}
				else if ("many-to-many" == tagName)
				{
					ManyToMany manyToMany;
					parseManyToMany(manyToMany, childElement);
					listMapping.setManyToMany(manyToMany);
				}
				else
				{
					throw HibernateException(QString("Unexpected tag(%1)").arg(tagName));
				}
				childElement = childElement.nextSiblingElement();
			}
		}
		else
		{
			throw HibernateException("'name' attribute expected.");
		}
	}

	void MappingParser::parseOneToManyMapping(OneToMany& oneToManyMapping, 
		const QDomElement& element)
	{
		QString className = element.attribute("class");
		oneToManyMapping.setClassName(className);

		if (element.hasAttribute("not-found"))
		{
			QString notFound = element.attribute("not-found");
			if ("exception" == notFound)
			{
				oneToManyMapping.setNotFound(OneToMany::eException);
			}
			else if ("ignore" == notFound)
			{
				oneToManyMapping.setNotFound(OneToMany::eIgnore);
			}
			else
			{
				throw HibernateException("'exception' or 'ignore' expected.");
			}
		}

		QString node = element.attribute("node");
		oneToManyMapping.setNode(node);

		if (element.hasAttribute("embed-xml"))
		{
			QString embedXml = element.attribute("embed-xml");
			if ("true" == embedXml)
			{
				oneToManyMapping.setEmbedXml(true);
			}
			else if ("false" == embedXml)
			{
				oneToManyMapping.setEmbedXml(false);
			}
			else
			{
				throw HibernateException("'true' or 'false' expected.");
			}		
		}

		QString entityName = element.attribute("entity-name");
		oneToManyMapping.setEntityName(entityName);
	}

	void MappingParser::parseKeyMapping(KeyMapping& keyMapping, 
		const QDomElement& element)
	{
		if (element.hasAttribute("column"))
		{
			QString column = element.attribute("column");
			ColumnMapping columnMapping;
			columnMapping.setName(column);
			keyMapping.addColumn(columnMapping);
		}
		else
		{
			QDomElement childElement = element.firstChildElement("column");

			while (!childElement.isNull())
			{
				QString tagName  = childElement.tagName();
				if ("column" == tagName)
				{
					ColumnMapping columnMapping;
					parseColumnMapping(columnMapping, childElement);
					keyMapping.addColumn(columnMapping);
				/*
				throw HibernateException(
					QString("'column' attribute expected for 'key' element at line %1 column %2.")
						.arg(element.lineNumber())
						.arg(element.columnNumber()));

				*/
				}
				childElement = childElement.nextSiblingElement("column");
			}
		}
	}

	void MappingParser::parseManyToMany(ManyToMany& manyToMany, 
		const QDomElement& element)
	{
		QString className = element.attribute("class");
		manyToMany.setClassName(className);

		if (element.hasAttribute("column"))
		{
			ColumnMapping columnMapping;
			columnMapping.setName(element.attribute("column"));
			if (element.hasAttribute("length"))
			{
				qint32 length = element.attribute("length").toInt();
				columnMapping.setLength(length);
			}

			manyToMany.addColumnMapping(columnMapping);
		}	
	}

	// void MappingParser::parseJoinedSubclass(JoinedSubclass& joinedSubclass, 
	// 	const QDomElement& element)
	// {
	// 	if (element.hasAttribute("name"))
	// 	{
	// 		QString className = element.attribute("name");
	// 		if (element.hasAttribute("table"))
	// 		{
	// 			joinedSubclass = JoinedSubclass(className);
	// 			QString tableName = element.attribute("table");
	// 			joinedSubclass.setTableName(tableName);
	// 
	// 			QDomElement childElement = element.firstChildElement();
	// 			while (!childElement.isNull())
	// 			{
	// 				QString tagName = childElement.tagName();
	// 				if (tagName == "key")
	// 				{
	// 					KeyMapping key;
	// 					parseKeyMapping(key, childElement);
	// 					joinedSubclass.setKey(key);
	// 				}
	// 				else if (tagName == "property")
	// 				{
	// 					PropertyMapping property;
	// 					parseProperty(property, childElement);
	// 					joinedSubclass.addProperty(property);
	// 				}
	// 
	// 				childElement = childElement.nextSiblingElement();
	// 			}
	// 		}
	// 	}
	// }

}
