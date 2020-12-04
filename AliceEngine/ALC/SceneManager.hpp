#ifndef ALC_SCENEMANAGER_HPP
#define ALC_SCENEMANAGER_HPP
#include "General.hpp"

namespace ALC {

	class SceneManager {
		ALC_NON_CONSTUCTABLE(SceneManager);
	public:

		template<typename Game>
		void Start();

	private:



	};

}

#endif // !ALC_SCENEMANAGER_HPP