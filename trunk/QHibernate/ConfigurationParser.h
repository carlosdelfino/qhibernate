
#ifndef QHibernate_ConfigurationParser
#define QHibernate_ConfigurationParser


class QString;
class QIODevice;

namespace QHibernate
{
	class SessionFactory;

	class ConfigurationParser
	{
	public:	

		void parseSessionFactory(SessionFactory& sessionFactory, 
			const QString& xmlContent);

		QString applySettings(const QString& text);

	};

}


#endif
