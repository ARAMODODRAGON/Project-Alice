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
#ifndef ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#define ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP
#include "Registry.hpp"

namespace ALC {

	struct EntityCreatorComponent final {

		// prepares creation code for an entity to be invoked later
		void Create(function<void(Entity)> callable);

		// marks an entity as destructable
		void Destroy(Entity e);

	private:
		vector<function<void(Entity)>> m_createCallables;
		vector<entt::entity> m_destructables;
	public:
		vector<function<void(Entity)>>& __GetCallables();
		vector<entt::entity>& __GetDestructables();
	};

	class EntityCreationHandler final {
	public:

		EntityCreationHandler();
		~EntityCreationHandler();

		// creates and destroys 
		void Cleanup(Registry& registry);

		// prepares creation code for an entity to be invoked later
		void Create(function<void(Entity)> callable);

		// mark entity as destructable
		void Destroy(Entity e);

	private:
		vector<function<void(Entity)>> m_createCallables;
		vector<entt::entity> m_destructables;
	};

}

#include "detail\EntityCreationHandler.inl"

#endif // !ALC_ENTITIES_ENTITYCREATIONHANDLER_HPP