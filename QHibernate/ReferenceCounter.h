
#ifndef QHibernate_ReferenceCounter
#define QHibernate_ReferenceCounter

namespace QHibernate
{
	template <class T>
	class ReferenceCounter
	{
	public: 

		ReferenceCounter()
			: mReferenceCount(0)
		{
		}

		void addReference()
		{
			mReferenceCount++;
		}

		void removeReference()
		{
			mReferenceCount--;
		}

		static void destroy(T* instance)
		{
			if (0 != instance)
			{
				if (0 == instance->mReferenceCount)
				{
					delete instance;
				}
			}
		}

		bool hasReference() const
		{
			return (mReferenceCount > 0);
		}

	private:

		int mReferenceCount;
	};
}

#endif
