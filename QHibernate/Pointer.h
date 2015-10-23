
#ifndef QHibernate_Pointer
#define QHibernate_Pointer

class QObject;

namespace QHibernate
{
	class PointerProxy
	{
	public:

		virtual ~PointerProxy()
		{

		}

		virtual void* getPointer() = 0;
	};


	struct PointerData
	{
	public:

		PointerData(PointerProxy* proxy)
			: mPointer(0)
			, mStrongRefCount(0)
			, mWeakRefCount(0)
			, mProxy(proxy)
		{
		}

		PointerData(QObject* pointer)
			: mPointer(pointer)
			, mStrongRefCount(0)
			, mWeakRefCount(0)
			, mProxy(0)
		{
		}

		~PointerData()
		{
			if (0 != mPointer)
			{
				delete mPointer;
			}

			if (0 != mProxy)
			{
				delete mProxy;
			}
		}


		QObject* getPointer()
		{
			if (0 != mProxy)
			{
				mPointer = reinterpret_cast<QObject*>(mProxy->getPointer());
				delete mProxy;
				mProxy = 0;
			}

			return mPointer;
		}

		inline void addStrongRef()
		{
			mStrongRefCount++;
		}

		inline void addWeakRef()
		{		
			mWeakRefCount++;	
		}

		inline void removeStrongRef()
		{
			mStrongRefCount--;
		}

		inline void removeWeakRef()
		{
			mWeakRefCount--;
		}

		inline int getStrongRefCount()
		{
			return mStrongRefCount;
		}

		inline int getWeakRefCount()
		{
			return mWeakRefCount;
		}

		void deletePointer()
		{
			if (0!= mPointer)
			{
				delete mPointer;
				mPointer = 0;
			}
			if (0 != mProxy)
			{
				delete mProxy;
				mProxy = 0;
			}
		}

	private:

		int mStrongRefCount;

		int mWeakRefCount;

		QObject* mPointer;

		PointerProxy* mProxy;
	};

	class CsBasePointer
	{
	public:

		virtual PointerData* getDataPointer() const = 0;

	protected:

		CsBasePointer()
			: mData(0)
		{
		}


		PointerData* mData;
	};


	template<class T>
	struct Pointer
		: public CsBasePointer
	{
	public:

		T* operator ->() const
		{
			T* pointer = 0;
			if (0 != mData)
			{
				pointer = qobject_cast<T*>(mData->getPointer());
			}

			return pointer;
		}

		T* getPointer() const
		{
			T* pointer = 0;
			if (0 != mData)
			{
				pointer = qobject_cast<T*>(mData->getPointer());
			}
			return pointer;
		}

		bool isNull() const
		{
			bool result = true;

			if (0 != mData)
			{
				if (0 != mData->getPointer())
				{
					result = false;
				}
			}

			return result;
		}


	protected:

		Pointer()
			: CsBasePointer()
		{
		}
	};
}

#endif
