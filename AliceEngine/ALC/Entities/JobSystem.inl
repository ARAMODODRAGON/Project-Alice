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