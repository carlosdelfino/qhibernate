
#include <QHibernate\HibernateException.h>

namespace QHibernate
{
	HibernateException::HibernateException(const QString& message)
	{
		mMessage = message;
		mStdMessage = message.toStdString();
	}

	const QString& HibernateException::getMessage() const
	{
		return mMessage;
	}

	const char* HibernateException::what() const
	{
		return mStdMessage.c_str();
	}
}
