#ifndef ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP
#define ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP

// declares that the struct or class is a component
#define ALC_Component public ::ALC::detail::component

namespace ALC {

	class Registry;
	class Behavior;

	namespace detail {

		struct component {
			virtual ~component() = 0 { }
		};

		template<typename T>
		using is_behavior_t = std::enable_if_t<std::is_base_of_v<Behavior, T>>;

		template<typename T>
		using is_component_t = std::enable_if_t<std::is_base_of_v<component, T>>;

		template<typename... T>
		struct are_components;

		template<typename T0, typename... T>
		struct are_components<T0, T...> {
			static constexpr bool value = std::is_base_of_v<component, T0> && are_components<T...>;
		};

		template<typename T>
		struct are_components<T> {
			static constexpr bool value = std::is_base_of_v<component, T>;
		};

		template<typename... T>
		using are_components_t = std::enable_if_t<are_components<T...>::value>;

		struct BehaviorList : ALC_Component {
			vector<Behavior*> behaviors;
		};

	}

}

#endif // !ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP