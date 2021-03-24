#ifndef ALC_JOBS_JOBQUEUE_HPP
#define ALC_JOBS_JOBQUEUE_HPP
#include "../General.hpp"
#include "cjs\cjs.hpp"

namespace ALC {

	// cleans up the usage of ijob and quick_fence to be included under the ALC namespace
	using IJob = cjs::ijob;
	using Fence = cjs::fence;
	using IFence = cjs::ifence;

	class JobQueue final {
		ALC_NON_CONSTRUCTABLE(JobQueue);
	public:

		// submits a job to be worked on
		static void Submit(IJob* job);

		// submits a function to be worked on
		static void Submit(void(*function)(void*), void* data = nullptr);

		// submits a fence to block the threads
		static void Submit(IFence* fence);

		// returns the number of active workers
		static uint32 GetWorkerCount();

		// awaits all jobs to be finished
		static void AwaitJobs();

	public:
		static void __Init(uint32 threadcount);
		static void __Exit();
	};

}

#endif // !ALC_JOBS_JOBQUEUE_HPP