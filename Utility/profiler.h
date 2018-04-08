#pragma once

namespace SOC_System {

	class ProNode
	{
	public:
		ProNode(char* _pFuncName = nullptr, char* _pItemName = nullptr)
			: pFuncName(_pFuncName)
			, pItemName(_pItemName)
			, callCount(0)
			, totalCallCount(0)
			, callCountAVG(0)
			, nowtime(0.0)
			, runtimeAVG(0.0)
			, runtime(0.0)
			, m_additionCount(0)
		{}

		void InitTotalCallCount() 
		{
			m_additionCount = 0;
			totalCallCount = 0; 
		}

		void InitCallCount() { callCount = 0; }
		void AddCallCount() { ++callCount; }
		__int64 GetCount() { return callCount; }
		void InitCountAVG() { callCountAVG = 0; }

		void SetNowTime(double time) { nowtime = time; }
		void SetElapsedTime(double time) 
		{
			AddCallCount();
			++totalCallCount;
			runtime += time;
			runtimeAVG = runtime / totalCallCount;

			nowtime += time;
			if (nowtime >= R_CONFIG().updateTime_Profiler)
			{
				nowtime = 0;

				++m_additionCount;
				InitCallCount();

				callCountAVG = totalCallCount / m_additionCount;
			}
		}

		double GetNowTime() { return nowtime; }

	private:
		__int64 m_additionCount;
	protected:
		char* pFuncName;
		char* pItemName;
		__int64 totalCallCount;
		__int64 callCount;
		__int64 callCountAVG;
		double runtime;
		double runtimeAVG;
		double nowtime;
	};

	class ProScope
	{
	public:
		ProScope(ProNode* pNode = nullptr)
			: m_pNode(pNode)
			, m_pTimer(nullptr)
		{
			if (pNode == nullptr)
				return;

			m_pTimer = new MPTimer();
			(m_pTimer != nullptr) ? m_pTimer->Start() : ((void)0);
		}

		virtual ~ProScope()
		{
			if (m_pNode == nullptr)
				return;
			if (m_pTimer == nullptr)
				return;

			m_pTimer->End();
			m_pNode->SetElapsedTime(m_pTimer->CalcRunTime());

			delete m_pTimer;
		}

	protected:
		MPTimer* m_pTimer;
		ProNode* m_pNode;
	};
}