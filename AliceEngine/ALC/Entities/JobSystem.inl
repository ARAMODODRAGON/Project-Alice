
template<typename ...Components>
inline ALC::JobSystem<Components...>::JobSystem(Registry& reg, const uint32 maxsubmitters)
	: m_reg(reg), m_view(m_reg.__GetReg().view<Components...>()) {
	m_submitters.resize(maxsubmitters);
	for (auto& sub : m_submitters) {
		sub.system = this;
		sub.view = &m_view;
	}
}

template<typename ...Components>
inline ALC::JobSystem<Components...>::~JobSystem() { }

template<typename ...Components>
inline void ALC::JobSystem<Components...>::StepSystem(Timestep ts, bool await) {

	Fence fence;

	uint32 i = 0;
	for (auto e : m_view) {
		if (i == m_submitters.size()) {
			JobQueue::Submit(&fence);
			fence.await_and_resume();
			fence.reset();
			i = 0;
		}
		auto& sub = m_submitters[i];
		sub.entity = Entity(e, &m_reg);
		sub.ts = ts;
		JobQueue::Submit(&sub);

	}

	if (await) {
		JobQueue::Submit(&fence);
		fence.await_and_resume();
	}
}

template<typename ...Components>
inline ALC::JobSystem<Components...>::Submitter::Submitter()
	: entity(nullptr), view(nullptr), system(nullptr) { }

template<typename ...Components>
inline void ALC::JobSystem<Components...>::Submitter::execute() {
	auto e = entity.__GetEntt();
	system->Step(ts, entity, view->get<Components>(e)...);
}