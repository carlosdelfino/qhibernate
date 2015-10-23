
#include <QHibernate/TypeQPointF.h>
#include <QHibernate/QHibernate.h>

#include <QtCore/QPointF>
#include <QtCore/QVariant>

namespace QHibernate
{
	TypeQPointF::TypeQPointF()
	{
	}

	TypeQPointF::~TypeQPointF()
	{
	}

	void TypeQPointF::setPropertyValue(QVariant& component,
		qint32 propertyIndex, 
		const QVariant& propertyValue) const
	{
		QPointF point = component.toPointF();
		switch(propertyIndex)
		{
		case 0:
			point.setX(propertyValue.toReal());
			break;
		case 1:
			point.setY(propertyValue.toReal());
			break;
		}

		component = QVariant(point);
	}

	QVariant TypeQPointF::getPropertyValue(const QVariant& component, qint32 propertyIndex) const
	{
		QVariant propertyValue;

		if (component.type() == QVariant::PointF)
		{
			QPointF point = component.toPointF();

			switch (propertyIndex)
			{
			case 0:
				propertyValue = QVariant(point.x());
				break;
			case 1:
				propertyValue = QVariant(point.y());
				break;
			}
		}

		return propertyValue;
	}

}

QHIBERNATE_CUSTOM_USER_TYPE(QPointF, QHibernate::TypeQPointF);