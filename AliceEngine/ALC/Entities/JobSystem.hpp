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