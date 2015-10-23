
#ifndef QHibernate_QHibernateExcepton
#define QHibernate_QHibernateExcepton

#include <QHibernate\QHibernate_global.h>
#include <QtCore\QString>

namespace QHibernate
{
	class QHIBERNATE_API HibernateException
		: public std::exception
	{
	public:

		HibernateException(const QString& message);

		const QString& getMessage() const;

		virtual const char* what() const;

	private:

		QString mMessage;

		std::string mStdMessage;
	};

}

#endif