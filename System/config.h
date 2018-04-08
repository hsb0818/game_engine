#pragma once

namespace SOC_System {

	class Config : public Singleton<Config>
	{
	public:
		Config() 
			: updateTime_Profiler(1.0)
			, isHighResolutionTimer(false)
		{ }

		void Init()
		{
			Init_Timer();
		}

	private:
		void Init_Timer()
		{
			// use possible ?
			isHighResolutionTimer = QueryPerformanceFrequency(&qpfFreq) != 0;
		}

	public:
		double updateTime_Profiler;
		LARGE_INTEGER qpfFreq;
		bool isHighResolutionTimer;
	};
}
