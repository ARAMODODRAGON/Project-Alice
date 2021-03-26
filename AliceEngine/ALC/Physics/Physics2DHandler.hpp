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
#ifndef ALC_PHYSICS_PHYSICS2DHANDLER_HPP
#define ALC_PHYSICS_PHYSICS2DHANDLER_HPP
#include "../General.hpp"
#include "../Entities/Registry.hpp"
#include "Rigidbody2D.hpp"

namespace ALC {

	class Physics2DHandler final {
		ALC_NO_COPY(Physics2DHandler);
	public:

		Physics2DHandler();
		~Physics2DHandler();

		// updates the physics
		void Step(Registry& registry, Timestep t);

	private:

		bool DoTrigger(Transform2D& t0, Rigidbody2D& r0,
					   Transform2D& t1, Rigidbody2D& r1);

		void UpdateCollisionInfo(EntityInfo& i0, Rigidbody2D& r0,
								 EntityInfo& i1, Rigidbody2D& r1);
	};

}

#endif // !ALC_PHYSICS_PHYSICS2DHANDLER_HPP