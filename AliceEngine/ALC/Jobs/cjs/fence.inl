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

namespace cjs {

	inline fence::fence()
		: m_shouldawait(false), m_done(false), m_shouldresume(false), m_joinedcount(0) { }

	inline fence::~fence() {
		await_and_resume();
	}

	inline void fence::await() {
		while (m_shouldawait && !m_done);
		m_shouldawait = false;
	}

	inline void fence::resume() {
		m_shouldresume = true;
		while (m_joinedcount > 0);
	}

	inline void fence::await_and_resume() {
		await();
		resume();
	}

	inline void fence::_submit() {
		await_and_resume();
		m_done = m_shouldresume = false;
		m_shouldawait = true;
	}

	inline void fence::_join() {
		++m_joinedcount;
		while (!m_shouldresume);
		--m_joinedcount;
	}

	inline void fence::_mark_done() {
		m_done = true;
	}
}