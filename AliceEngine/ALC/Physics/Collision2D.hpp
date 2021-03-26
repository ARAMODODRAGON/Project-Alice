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
#ifndef ALC_PHYSICS_COLLISION2D_HPP
#define ALC_PHYSICS_COLLISION2D_HPP
#include "Rigidbody2D.hpp"
#include <glm\gtx\norm.hpp>

namespace ALC {
	namespace Collision2D {

		bool CircleCircleTrig(Transform2D& t0, Rigidbody2D& r0, CircleShape& c0,
							  Transform2D& t1, Rigidbody2D& r1, CircleShape& c1);

	}
}

inline bool ALC::Collision2D::CircleCircleTrig(
	Transform2D& t0, Rigidbody2D& r0, CircleShape& c0, 
	Transform2D& t1, Rigidbody2D& r1, CircleShape& c1) {
	vec2 p0 = t0.position + c0.offset;
	vec2 p1 = t1.position + c1.offset;
	float dist = c0.radius + c1.radius;
	return glm::length2(p0 - p1) < (dist * dist) ;
}

#endif // !ALC_PHYSICS_COLLISION2D_HPP