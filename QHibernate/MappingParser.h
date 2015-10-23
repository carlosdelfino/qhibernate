#ifndef QHibernate_ClassMappingParser
#define QHibernate_ClassMappingParser

#include <QtCore\QSharedPointer>

class QString;
class QIODevice;
class QDomElement;

namespace QHibernate
{
	class Class;
	class IdentifierMapping;
	class ColumnMapping;
	class PropertyMapping;
	class ManyToOne;
	class OneToOne;
	class Relationship;
	class SessionFactory;
	class ListMapping;
	class OneToMany;
	class KeyMapping;
	class GeneratorMapping;
	class ManyToMany;
	class JoinedSubclass;

	class MappingParser
	{
	public:

		MappingParser();

		void parseMapping(const QString& filePath, 
			SessionFactory& sessionFactory);

		void parseMapping(QIODevice* device, 
			SessionFactory& sessionFactory);	

	private:

		void parseClassMapping(Class& classMapping, 
			const QDomElement& element);

		void parseIdentifierMapping(IdentifierMapping& identifierMapping, 
			const QDomElement& element);

		void parseProperty(PropertyMapping& property, 
			const QDomElement& element);

		void parseColumnMapping(ColumnMapping& columnMapping, 
			const QDomElement& element);

		//void parseRelationship(Relationship& relationship, const QDomElement& element);

		void parseManyToOne(ManyToOne& manyToOne, const QDomElement& element);

		void parseOneToOne(OneToOne& oneToOne, const QDomElement& element);

		void parseListMapping(ListMapping& listMapping, const QDomElement& element);

		void parseOneToManyMapping(OneToMany& oneToManyMapping, 
			const QDomElement& element);

		void parseJoinedSubclass(JoinedSubclass& joinedSubclass, 
			const QDomElement& element);

		void parseKeyMapping(KeyMapping& keyMapping, 
			const QDomElement& element);

		void parseGeneratorMapping(GeneratorMapping& generatorMapping, 
			const QDomElement& element);

		void parseManyToMany(ManyToMany& manyToMany, const QDomElement& element);
	};

}


#endif
