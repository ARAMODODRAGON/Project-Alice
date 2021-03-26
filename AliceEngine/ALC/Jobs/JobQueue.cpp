/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "JobQueue.hpp"
#include "cjs\cjs.hpp"

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

	uint32 JobQueue::GetWorkerCount() {
		return g_threadCount;
	}

	void JobQueue::AwaitJobs() {
		//if (g_queue.size() > 0) {
		//	while (g_queue.size() > 0);
		//}
		Fence fence;
		Submit(&fence);
		fence.await_and_resume();
	}

	void JobQueue::__Init(uint32 threadcount) {
		// garuntees that there are at least 1 thread if none are specified
		if (threadcount == 0)
			threadcount = glm::max(int(cjs::thread::hardware_concurrency() - 1), 1);
		g_threadCount = threadcount;
		g_threads.reset(new cjs::worker_thread[g_threadCount]);
		for (size_t i = 0; i < g_threadCount; ++i) g_threads.get()[i].attach_to(&g_queue);
	}

	void JobQueue::__Exit() {
		g_threads.reset();
		g_threadCount = 0;
	}

}