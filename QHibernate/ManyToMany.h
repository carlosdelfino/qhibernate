
#ifndef QHibernate_ManyToMany
#define QHibernate_ManyToMany

#include <QtCore/QString>
#include <QtCore/QVector>

#include <QHibernate/ColumnMapping.h>

namespace QHibernate
{
	class ManyToMany
	{
	public:

		ManyToMany();	

		bool isValid() const;

		QString getClassName() const;

		void setClassName(const QString& className);

		void addColumnMapping(const ColumnMapping& columnMapping);

		const QVector<ColumnMapping>& getColumnMappings() const;

	private:

		QString mClassName;

		QVector<ColumnMapping> mColumnMappings;
	};
}

#endif
