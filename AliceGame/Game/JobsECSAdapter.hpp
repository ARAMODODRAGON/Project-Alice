#ifndef ALICE_JOBSECSADAPTER_HPP
#define ALICE_JOBSECSADAPTER_HPP
#include <ALC\Jobs.hpp>
#include <ALC\Entities.hpp>

class JobsECSAdapter final {
public:

	JobsECSAdapter(ALC::Registry& registry);
	~JobsECSAdapter();

	// runs jobs on the given system, divides the entities into chunks for each job to run through
	// uses the calling thread to run a chunk as well
	template<typename... Params>
	void ApplyJobs(ALC::Timestep ts, ALC::ISystem<Params...>& system);

private:

	using adpfunc_t = void(*)(ALC::Registry*, ALC::Timestep, void*, ALC::uint32, ALC::uint32);

	struct JobAdapter final : ALC::IJob {
		void* system = nullptr;
		ALC::uint32 beginrange = 0;
		ALC::uint32 endrange = 0;
		ALC::Timestep ts;
		ALC::Registry* reg = nullptr;
		adpfunc_t adpfunc = nullptr;
		void execute() override;
	};

	ALC::Registry* m_reg;
	ALC::vector<JobAdapter> m_adpjobs;

};

#include "JobsECSAdapter.inl"

#endif // !JOBSECSADAPTER_HPP