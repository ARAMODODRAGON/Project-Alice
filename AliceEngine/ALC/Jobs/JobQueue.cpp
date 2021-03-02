#include "JobQueue.hpp"

namespace ALC {

	namespace {

		Scope<cjs::worker_thread[]> g_threads;
		size_t g_threadCount;
		cjs::work_queue g_queue;

	}

	void JobQueue::Submit(IJob* job) {
		#ifdef _DEBUG
		if (g_threadCount == 0) {
			ALC_DEBUG_ERROR("JobSystem has not been enabled!");
			return;
		}
		#endif
		g_queue.submit(job);
	}

	void JobQueue::Submit(void(*function)(void*), void* data) {
		#ifdef _DEBUG
		if (g_threadCount == 0) {
			ALC_DEBUG_ERROR("JobSystem has not been enabled!");
			return;
		}
		#endif
		g_queue.submit(function, data);
	}

	void JobQueue::Submit(IFence* fence) {
		#ifdef _DEBUG
		if (g_threadCount == 0) {
			ALC_DEBUG_ERROR("JobSystem has not been enabled!");
			return;
		}
		#endif
		g_queue.submit(fence);
	}

	void JobQueue::__Init(uint32 threadcount) { 
		// garuntees that there are at least 1 thread if none are specified
		if (threadcount == 0)
			threadcount = glm::max(int(cjs::thread::hardware_concurrency() - 1), 2);
		g_threadCount = threadcount;
		g_threads.reset(new cjs::worker_thread[g_threadCount]);
		for (size_t i = 0; i < g_threadCount; ++i) g_threads.get()[i].attach_to(&g_queue);
	}

	void JobQueue::__Exit() { 
		g_threads.reset();
		g_threadCount = 0;
	}

}