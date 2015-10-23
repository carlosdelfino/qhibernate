
#ifndef QHibernate_KeyMapping
#define QHibernate_KeyMapping

#include <QtCore/QVector>
#include <QHibernate/ColumnList.h>

namespace QHibernate
{
	struct CsKeyData;

	class KeyMapping
	{
	public:

		KeyMapping();

		KeyMapping(const KeyMapping& key);

		~KeyMapping();

		KeyMapping& operator = (const KeyMapping& key);

		ColumnList getColumnList() const;

		void addColumn(const ColumnMapping& column);

		qint32 getColumnCount() const;

		ColumnMapping getColumn(qint32 index) const;

		bool isValid() const;

	private:

		CsKeyData* mData;

		void setData(CsKeyData* data);

	};
}

#endif
