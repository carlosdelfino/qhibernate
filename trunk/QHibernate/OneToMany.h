
#ifndef QHibernate_OneToManyMapping
#define QHibernate_OneToManyMapping

#include <QtCore/QString>

namespace QHibernate
{
	class OneToMany
	{
	public:

		OneToMany();

		enum NotFound
		{
			eIgnore,
			eException
		};

		const QString& getClassName() const;
		void setClassName(const QString& className);

		NotFound getNotFound() const;
		void setNotFound(NotFound notFound);

		const QString& getNode() const;
		void setNode(const QString& node);

		void setEmbedXml(bool isEmbedXml);
		bool isEmbedXml() const;

		const QString& getEntityName() const;
		void setEntityName(const QString& entityName);

		bool isValid() const;

	private:

		QString mClassName;

		NotFound mNotFound;

		QString mNode;

		bool mEmbedXml;

		QString mEntityName;
	};

}


#endif
