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
#ifndef ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP
#define ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP

namespace ALC {

	class Registry;
	class Behavior;

	namespace detail {

		template<typename T>
		using is_behavior_t = std::enable_if_t<std::is_base_of_v<Behavior, T>>;

		//template<typename T>
		//using is_component_t = std::enable_if_t<std::is_base_of_v<component, T>>;

		//template<typename... T>
		//struct are_components;
		//
		//template<typename T0, typename... T>
		//struct are_components<T0, T...> {
		//	static constexpr bool value = std::is_base_of_v<component, T0> && are_components<T...>;
		//};
		//
		//template<typename T>
		//struct are_components<T> {
		//	static constexpr bool value = std::is_base_of_v<component, T>;
		//};
		//
		//template<typename... T>
		//using are_components_t = std::enable_if_t<are_components<T...>::value>;

		struct BehaviorList {
			vector<Behavior*> behaviors;
		};

	}

}

#endif // !ALC_ENTITIES_DETAIL_COMPONENTTYPEINFO_HPP