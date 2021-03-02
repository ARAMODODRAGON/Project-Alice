#ifndef ALC_ENTITIES_JOBSYSTEM_HPP
#define ALC_ENTITIES_JOBSYSTEM_HPP
#include "Registry.hpp"
#include "../Jobs.hpp"

namespace ALC {

	template<typename... Components>
	struct JobSystem {

		// constructs this jobsystem with a number of submitters (used to limit the amount of jobs)
		JobSystem(Registry& reg, const uint32 maxsubmitters = 100);
		~JobSystem();

		void StepSystem(Timestep ts, bool await = true);

		virtual void Step(Timestep ts, Entity e, Components&... components) = 0;

	private:
		using view_type = entt::view<entt::exclude_t<>, Components...>;
		struct Submitter final : public IJob {
			Submitter();
			void execute() override;
			Entity entity;
			view_type* view;
			JobSystem* system;
			Timestep ts;
		};
		friend JobSystem::Submitter;
		
		Registry& m_reg;
		view_type m_view;
		vector<Submitter> m_submitters;
	};

}

#include "JobSystem.inl"

#endif // !ALC_ENTITIES_JOBSYSTEM_HPP