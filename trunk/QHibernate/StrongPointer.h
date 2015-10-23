
#ifndef QHibernate_StrongPointer
#define QHibernate_StrongPointer

#include <QHibernate/Pointer.h>

namespace QHibernate
{
	template <class T> class StrongPointer
		: public Pointer<T>
	{
	public:

		StrongPointer()
			: Pointer<T>()
		{
		}

		StrongPointer(T* pointer)
			: Pointer<T>()
		{
			setData(new PointerData(pointer));		
		}

		StrongPointer(PointerProxy* proxy)
			: Pointer<T>()
		{
			setData(new PointerData(proxy));
		}

		StrongPointer(const Pointer<T>& pointer)
			: Pointer<T>()
		{
			*this = pointer;
		}

		StrongPointer(const StrongPointer<T>& strongPointer)
			: Pointer<T>()
		{
			*this = strongPointer;
		}

		template<class X>
		StrongPointer(const Pointer<X>& pointer)
			: Pointer<T>()
		{
			X* x = pointer.getPointer();
			T* t = dynamic_cast<T*>(x);

			if (0 != t)
			{
				setData(pointer.getDataPointer());
			}
			else
			{
				setData(0);
			}
		}

		~StrongPointer()
		{
			setData(0);
		}

		StrongPointer<T>& operator = (const CsBasePointer& pointer)
		{
			setData(pointer.getDataPointer());
			return *this;
		}

		StrongPointer<T>& operator = (const StrongPointer<T>& strongPointer)
		{
			setData(strongPointer.mData);
			return *this;
		}

		virtual PointerData* getDataPointer() const
		{
			return mData;
		}

	private:

		void setData(PointerData* data)
		{
			if (0 != mData)
			{
				mData->removeStrongRef();
				if (0 == mData->getStrongRefCount())
				{
					mData->deletePointer();
					if (0 == mData->getWeakRefCount())
					{
						delete mData;

						mData = 0;
					}
				}			
			}

			mData = data;

			if (0 != mData)
			{
				mData->addStrongRef();
			}
		}	
	};

}


#endif
