
inline JobsECSAdapter::JobsECSAdapter(ALC::Registry& registry) : m_reg(&registry) {
	m_adpjobs.resize(ALC::JobQueue::GetWorkerCount());
}

inline JobsECSAdapter::~JobsECSAdapter() { }

template<typename... Params>
inline void JobsECSAdapter::ApplyJobs(ALC::Timestep ts, ALC::ISystem<Params...>& system) {

	// make sure the pools exist
	//(m_reg.__GetReg().prepare<Params>(), ...);

	//using first_t = std::tuple_element_t<0, std::tuple<Params...>>;
	//using get_t = entt::get_t<Params...>;
	using group_t = entt::group<entt::exclude_t<>, entt::get_t<std::decay_t<Params>...>>;
	using system_t = ALC::ISystem<Params...>;

	// adapts the jobs to use the given template params
	adpfunc_t ADPFunc =
		[](ALC::Registry* registry, ALC::Timestep ts, void* system_, ALC::uint32 begin, ALC::uint32 end) -> void {
		// reinterpret to the correct types
		system_t* system = reinterpret_cast<system_t*>(system_);
		entt::registry* reg = &(registry->__GetReg());
		group_t group = reg->group<>(entt::get_t<Params...>{});
		const entt::entity* entities = group.data();

		// loop
		for (size_t i = begin; i < end; i++) {
			ALC::Entity e(entities[i], registry);
			system->Step(ts, e, group.get<Params>(entities[i])...);
		}
	};

	group_t group = m_reg->__GetReg().group<>(entt::get_t<Params...>{});
	if (group.size() == 0) return;
	const entt::entity* entities = group.data();

	// get worker count and chunk size
	const size_t groupsz = group.size();
	const ALC::uint32 chunksize = groupsz / (m_adpjobs.size() + 1);

	ALC::uint32 begin = 0;

	// fill the jobs and submit
	if (chunksize > 1) {
		for (size_t i = 0; i < m_adpjobs.size(); i++) {
			auto& job = m_adpjobs[i];

			// set basic data
			job.system = &system;
			job.ts = ts;
			job.reg = m_reg;
			job.adpfunc = ADPFunc;

			// this is so one of the jobs will always have any extra entities outside the chunk
			ALC::int32 chunk = chunksize;

			// set range
			if (i == 0) chunk += groupsz % (m_adpjobs.size() + 1);
			job.beginrange = begin;
			job.endrange = job.beginrange + chunk;
			begin += chunk;

			// submit
			ALC::JobQueue::Submit(&job);
		}
	}

	// do the remaining work on this thread
	entt::registry* reg = &(m_reg->__GetReg());
	for (size_t i = begin; i < groupsz; i++) {
		ALC::Entity e(entities[i], m_reg);
		system.Step(ts, e, group.get<Params>(entities[i])...);
	}

	ALC::JobQueue::AwaitJobs();

}

inline void JobsECSAdapter::JobAdapter::execute() {
	if (reg && adpfunc && system && (endrange > beginrange)) {
		adpfunc(reg, ts, system, beginrange, endrange);
		reg = nullptr;
		adpfunc = nullptr;
		system = nullptr;
		endrange = beginrange = 0;
	}
}