#pragma once

namespace SOC_System {

	// MP is Memory Pool
	class MPTimer : public SOC_Memory::BaseMP
	{
	public:
		MPTimer() { this->Start(); }
		virtual ~MPTimer() {} 
		// don't need to call. and virtual is create 4byte's table.
		// but there are my memory pool.
	private:
		MPTimer(const MPTimer& rhs); // No implement

	public:
		__int64 GetCurTime(LARGE_INTEGER* pTime)
		{
			if (pTime == nullptr)
				return -1;

			if (R_CONFIG().isHighResolutionTimer)
			{
				QueryPerformanceCounter(pTime);
				return pTime->QuadPart;
			}
			else
			{
				pTime->QuadPart = timeGetTime();
				return pTime->QuadPart;
			}
		}

		// hmm... default(unit is 1) is second.
		double CalcRunTime(__int64 unit = 1)
		{
			if (unit <= 0)
				unit = 1;

			if (R_CONFIG().isHighResolutionTimer)
			{
				return ( (double)(m_end.QuadPart - m_start.QuadPart) ) / ( (double)(R_CONFIG().qpfFreq.QuadPart / unit) );
			}
			else
			{
				unit *= 1000;
				return ( (double)(m_end.QuadPart - m_start.QuadPart) ) / ((double)(unit));
			}
		}

		void EndisStart() { m_end.QuadPart = m_start.QuadPart; }
		void StartisEnd() { m_start.QuadPart = m_end.QuadPart; }
		__int64 Start() { return GetCurTime(&m_start); }
		__int64 End() { return GetCurTime(&m_end); }
		__int64& GetStartTime() { return m_start.QuadPart; }
		__int64& GetEndTime() { return m_end.QuadPart; }

	private:
		LARGE_INTEGER m_start;
		LARGE_INTEGER m_end;
	};
}