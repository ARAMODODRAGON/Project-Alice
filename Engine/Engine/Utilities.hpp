#ifndef _UTILITIES_HPP
#define _UTILITIES_HPP
#include "GeneralMacros.hpp"

struct Util {
	NON_CONSTRUCTABLE(Util);
	
	struct App {
		NON_CONSTRUCTABLE(App);

		static void Run();
		static void Quit();
	};

};

#endif // !_UTILITIES_HPP