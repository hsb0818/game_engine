#ifndef _SYS_INSTANCE_RETURNER_H_
#define _SYS_INSTANCE_RETURNER_H_

using namespace SOC_System;

static Thread::ThreadPool& R_THREAD_POOL()
{
	return Thread::ThreadPool::GetInstance();
}

static Config& R_CONFIG()
{
	return Config::GetInstance();
}

#endif