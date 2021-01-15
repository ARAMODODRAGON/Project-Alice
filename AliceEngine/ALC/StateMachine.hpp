#ifndef ALC_STATEMACHINE_HPP
#define ALC_STATEMACHINE_HPP
#include "General.hpp"

namespace ALC {

	/// <summary>
	/// handler to hold the current state and the next state to switch to
	/// </summary>
	/// <typeparam name="Class">the class that this statemachine will invoke functions on</typeparam>
	/// <typeparam name="...Params"></typeparam>
	/// <typeparam name="StateTy"></typeparam>
	template<typename Class, typename StateTy, typename... Params>
	class StateMachine final {
		ALC_NO_COPY(StateMachine);
	public:

		// the integer convertable type used for the state
		using Statetype = StateTy;

		// called during that state
		using StateFunc = void(Class::*)(Params...);

		// called when switching away from state
		// param is the last state
		using BeginStateFunc = void(Class::*)(const Statetype, Params...);

		// defaults the state to 0
		StateMachine(Class* instance)
			: m_instance(instance)
			, m_currentState(static_cast<Statetype>(0))
			, m_nextState(m_currentState) {
			ALC_ASSERT(instance, "instance must be true");
		}

		// sets the state 
		StateMachine(Class* instance, Statetype state)
			: m_instance(instance)
			, m_currentState(state)
			, m_nextState(m_currentState) {
			ALC_ASSERT(instance, "instance must be true");
		}

		~StateMachine() { }

		// adds a state to the statemachine
		void Add(const Statetype state, StateFunc func) {
			// bind only once
			ALC_ASSERT(m_boundStates.find(state) == m_boundStates.end(), "state must not already be set");
			// set the state
			m_boundStates.emplace(state, func);
		}

		// adds a begin state to the statemachine
		void Add(const Statetype state, BeginStateFunc func) {
			// bind only once
			ALC_ASSERT(m_boundBeginStates.find(state) == m_boundBeginStates.end(), "beginstate must not already be set");
			// set the state
			m_boundBeginStates.emplace(state, func);
		}

		// removes a state from the statemachine
		void RemoveState(const Statetype state) {
			auto it = m_boundStates.find(state);
			if (it != m_boundStates.end())
				m_boundStates.erase(it);
		}

		// removes a begin state from the statemachine
		void RemoveBeginState(const Statetype state) {
			auto it = m_boundBeginStates.find(state);
			if (it != m_boundBeginStates.end())
				m_boundBeginStates.erase(it);
		}

		// returns the current state
		Statetype GetState() const { return m_currentState; }

		// sets the next state to be called
		void ChangeState(const Statetype state) {
			m_nextState = state;
		}

		// invokes the current state
		void operator()(Params... params) {

			if (m_currentState != m_nextState) {
				auto it = m_boundBeginStates.find(m_nextState);
				// invoke change state
				if (it != m_boundBeginStates.end())
					(m_instance->*(it->second))(m_currentState, params...);
				m_currentState = m_nextState;
			}

			// state must be bound
			ALC_ASSERT(m_boundStates.find(m_currentState) != m_boundStates.end(), "callbacks must be set");

			// call
			(m_instance->*(m_boundStates[m_currentState]))(params...);

		}

	private:

		Class* m_instance;
		Statetype m_currentState;
		Statetype m_nextState;
		unordered_map<Statetype, StateFunc> m_boundStates;
		unordered_map<Statetype, BeginStateFunc> m_boundBeginStates;

	};

}

#endif // !ALC_STATEMACHINE_HPP