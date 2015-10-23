

#ifndef QHibernate_CsProperty
#define QHibernate_CsProperty

#include <QHibernate/QHibernate_global.h>
#include <QHibernate/ColumnMapping.h>

#include <QtCore/QString>
#include <QtCore/QVector>

namespace QHibernate
{
	class PropertyMapping
	{
	public:

		PropertyMapping();

		void setName(const QString& name);

		const QString& getName() const;

		void setType(const QString& type);

		QString getType() const;

		const QVector<ColumnMapping>& getColumnMappings() const;

		void addColumnMapping(const ColumnMapping& columnMapping);

		qint32 getColumnCount() const;

		ColumnMapping getColumn(qint32 index) const;

	protected:

	private:

		QString mType;

		QString mName;

		QVector<ColumnMapping> mColumnMappings;
	};
}


#endif
