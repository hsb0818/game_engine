#ifndef _MEM_INSTANCE_RETURNER_
#define _MEM_INSTANCE_RETURNER_

using namespace SOC_Memory;

static MemoryPool<MIN_EXP, MAX_EXP, BUFFER_SIZE>& R_MEMORYPOOL()
{
	return MemoryPool<MIN_EXP, MAX_EXP, BUFFER_SIZE>::GetInstance(); 
}


namespace SOC_Memory {

	class BaseMP
	{
	public:
		BaseMP() {}
		virtual ~BaseMP() {}

	public:
		void* operator new(size_t size)
		{
			if (size == 0)
				return nullptr;

			printf("%d - allocated\n", size);
			return R_MEMORYPOOL().Alloc(size);
		}

		void* operator new[](size_t size)
		{
			if (size == 0)
				return nullptr;

			printf("%d - allocated\n", size);
			return R_MEMORYPOOL().Alloc(size);
		}

		void operator delete(void* p)
		{
			if (p == nullptr)
				return;

			printf("%p - delete\n", p);
			R_MEMORYPOOL().Free(p);
		}

		void operator delete[](void* p)
		{
			if (p == nullptr)
				return;

			printf("%p - delete\n", p);
			R_MEMORYPOOL().Free(p);
		}
	};

}

#endif