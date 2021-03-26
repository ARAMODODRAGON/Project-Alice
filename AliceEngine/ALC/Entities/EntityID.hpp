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
#ifndef ALC_ENTITIES_ID_HPP
#define ALC_ENTITIES_ID_HPP
#include "../General.hpp"

namespace ALC {

	struct EntityID {

		EntityID() : m_ID(-1) { }

		EntityID(std::nullptr_t) : m_ID(-1) { }

		// returns true if this is a valid UID
		bool IsValid() const { return m_ID != -1; }

		// returns true if this is a valid UID
		operator bool() const { return m_ID != -1; }

		// compares the two entity UIDs
		bool operator==(const EntityID& other) {
			return m_ID == other.m_ID;
		}

		// compares the two entity UIDs
		bool operator!=(const EntityID& other) {
			return m_ID == other.m_ID;
		}

		explicit EntityID(const uint64 ID_) ALC_INTERNAL
			: m_ID(ID_) { }

	private:
		uint64 m_ID;
	};

}

#endif // !ALC_ENTITIES_ID_HPP