//
//  Lock.h
//  Created by SungBin_Hong on 13. 10. 13..
//  Copyright (c) 2013 years Teraphonia. All rights reserved.
//

#pragma once

namespace SOC_System {
namespace Lock {

	// 'I' mean is interface
	class ILock
	{
	public:
		ILock() {}
		virtual ~ILock() {}
		
		virtual void Enter(const char* pFileName, int line) = 0;
		virtual void Leave() = 0;
		
		virtual void ReadLock(const char* pFileName, int line)
		{
			Enter(pFileName, line);
		}
		virtual void ReadUnlock()
		{
			Leave();
		}
		virtual void WriteLock(const char* pFileName, int line)
		{
			Enter(pFileName, line);			
		}
		virtual void WriteUnlock()
		{
			Leave();
		}
	};

	class CriticalSectionLock : public ILock
	{
	public:
		CriticalSectionLock(unsigned int spinCount = 0);
		virtual ~CriticalSectionLock();

		virtual void Enter(const char* pFileName, int line);
		virtual void Leave();

	private:
#if defined(_WIN32) || defined(_WIN64)
		CRITICAL_SECTION m_key;
#else
		pthread_mutex_t m_key;
#endif
	};

	namespace SpinLock
    {
        class DefSpinLock
        {
        public:
            DefSpinLock(unsigned int spincount = 3000);
            virtual ~DefSpinLock();

            virtual void Enter(const char* pFileName, int line);
            virtual void Leave();
            
        private:
#if defined(_WIN32) || defined(_WIN64)
            // m_key;
			
#else
            OSSpinLock m_key;
#endif
        };
        
//		class BackoffLock : public ILock
	}

	namespace Helper {
		class ScopedLock
		{
		private:
			ScopedLock& operator=(const ScopedLock &);
		public:
			ScopedLock(ILock& lock, const char* pFileName = nullptr, int line = 0) : m_lock(lock)
			{
				m_lock.Enter(pFileName, line);
			}
			~ScopedLock()
			{
				m_lock.Leave();
			}

		private:
			ILock& m_lock;
		};

		template<typename T>
		class TypedScopedLock
		{
		private:
			TypedScopedLock& operator=(const TypedScopedLock &);
		public:
			TypedScopedLock(T& lock, const char* pFileName = nullptr, int line = 0) : m_lock(lock)
			{
				m_lock.Enter(pFileName, line);
			}
			~TypedScopedLock()
			{
				m_lock.Leave();
			}

		private:
			T& m_lock;
		};
	}

}
}

#define TYPED_SCOPE_LOCK(lock)  SOC_System::Lock::Helper::TypedScopedLock<SOC_System::Lock::CriticalSectionLock> __lock##__LINE__##(lock, __FILE__, __LINE__)
#define TYPED_SCOPE_SPINLOCK(lock)  SOC_System::Lock::Helper::TypedScopedLock<SOC_System::Lock::SpinLock::DefSpinLock> __lock##__LINE__##(lock, __FILE__, __LINE__)
