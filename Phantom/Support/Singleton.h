//--------------------------------------------------------------------------------------
//		A thread Safe Singleton
//		Based on code from 
//		http://www.opbarnes.com/blog/Programming/OPB/Snippets/Singleton.html
//--------------------------------------------------------------------------------------
#ifndef	SINGLETON_H
#define	SINGLETON_H

template <typename T>
class CSingletonT
{
private:
	CSingletonT();
	~CSingletonT();
	const CSingletonT& operator=(const CSingletonT&	src);

protected:
	static volatile	LONG m_lLocker;

	static T* GetWkr()
	{
		T* pTmp	= NULL;
		try
		{
			static T tVar;
			pTmp = &tVar;
		}
		catch(...)
		{
			_ASSERT(FALSE);
			pTmp = NULL;
		}
		return pTmp;
	}

public:
	static T* Get()
	{
		while (::InterlockedExchange(&m_lLocker, 1)	!= 0)
		{
			Sleep(1);
		}
		T* pTmp	= GetWkr();
		::InterlockedExchange(&m_lLocker, 0);
		return pTmp;
	}
};

template <typename T>
volatile LONG CSingletonT<T>::m_lLocker	= 0;

#endif //SINGLETON_H
