#pragma once

namespace SOC_Utill {
	using namespace SOC_System;

class MemProNode : public ProNode
{
public:
	MemProNode(char* _pFuncName = nullptr, char* _pItemName = nullptr)
		: ProNode(_pFuncName, _pItemName)
		, perOfMemory(0)
		, totalMemory(0)
	{}

	int perOfMemory;
	__int64 totalMemory;
};

class MemoryProfiler : public CLog
{
public:
	typedef std::deque<MemProNode*> CallStack;

private:
	static MemoryProfiler* m_pInst;
public:
	static MemoryProfiler& GetInstance()
	{
		if (m_pInst == nullptr)
			m_pInst = new MemoryProfiler;

		return *m_pInst;
	}

public:
	virtual void Add(int log_type, const WCHAR* format, ...);
	CallStack& GetCallStack() { return m_callStack; }

private:
	CallStack m_callStack;
};

}

static SOC_Utill::MemoryProfiler& MEM_PROFILER()
{
	return SOC_Utill::MemoryProfiler::GetInstance();
}

using namespace SOC_Utill;

class MemProScope : public ProScope
{
public:
	MemProScope(MemProNode* pNode = nullptr) // not ProNode
		: ProScope(pNode)
	{
		if (pNode == nullptr)
			return;

		MEM_PROFILER().GetCallStack().push_back(pNode); // non null is start sign.
	}

	~MemProScope()
	{
		if (m_pNode == nullptr)
			return;

		MEM_PROFILER().GetCallStack().push_back(nullptr); // null is end sign.
	}
};

#ifdef __PROFILE_MODE__
	#define PROFILE_ITEM_MEMORY_AUTO \
		static MemProNode __ProNode(__func__, "None");	\
		MemProScope __ProfileScope(&__ProNode);	\

#define PROFILE_ITEM_MEMORY_AUTO_NODE (__ProNode)

	#define PROFILE_ITEM_MEMORY(item) \
		static MemProNode __ProNode_##item(__func__, #item);	\
		MemProScope __ProfileScope_##item(&__ProNode_##item);	\

#define PROFILE_ITEM_MEMORY_NODE(item) (__ProNode_##item)
		
#else
	#define PROFILE_ITEM_MEMORY(item)		((void)0)
	#define PROFILE_ITEM_MEMORY_AUTOSETTING	((void)0)

#endif