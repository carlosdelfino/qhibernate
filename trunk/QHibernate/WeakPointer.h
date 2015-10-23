
#ifndef QHibernate_WeakPointer
#define QHibernate_WeakPointer

#include <QHibernate/Pointer.h>

namespace QHibernate
{
	template <class T>
	class WeakPointer
		: public Pointer<T>
	{
	public:

		WeakPointer()
			: Pointer<T>()
		{
		}

		WeakPointer(T* pointer)
			: Pointer<T>()
		{
			setData(new PointerData(pointer))
		}

		WeakPointer(PointerProxy* proxy)
			: Pointer<T>()
		{
			setData(new PointerData(proxy));
		}

		WeakPointer(const Pointer<T>& pointer)
			: Pointer<T>()
		{
			*this = pointer;
		}

		WeakPointer(const WeakPointer<T>& strongPointer)
			: Pointer<T>()
		{
			*this = strongPointer;
		}

		template<class X>
		WeakPointer(const Pointer<X>& pointer)
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

		~WeakPointer()
		{
			setData(0);
		}

		WeakPointer<T>& operator = (const Pointer<T>& pointer)
		{
			setData(pointer.getDataPointer());
			return *this;
		}

		WeakPointer<T>& operator = (const WeakPointer<T>& weakPointer)
		{
			setData(weakPointer.mData);
			return *this;
		}

		virtual PointerData* getDataPointer() const
		{
			return mData;
		}

	private:

		virtual void setData(PointerData* data)
		{
			if (0 != mData)
			{
				mData->addWeakRef();
				if (0 == mData->getWeakRefCount())
				{				
					if (0 == mData->getStrongRefCount())
					{
						delete mData;

						mData = 0;
					}
				}
			}

			mData = data;

			if (0 != mData)
			{
				mData->addWeakRef();
			}
		}

	};
}

#endif
