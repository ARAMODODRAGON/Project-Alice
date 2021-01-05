#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include <glm\glm.hpp>
#include <glm\gtx\string_cast.hpp>
#include <string>

// disables the copy constructor and operator
#define ALC_NO_COPY(TYPE)				\
TYPE(const TYPE&) = delete;				\
TYPE& operator=(const TYPE&) = delete;		

// disables the move constructor and operator
#define ALC_NO_MOVE(TYPE)				\
TYPE(TYPE&&) = delete;					\
TYPE& operator=(TYPE&&) = delete;

// disables copy/move constructors and operators
// and disables the default constructor and destructor
#define ALC_NON_CONSTRUCTABLE(TYPE)		\
ALC_NO_COPY(TYPE) ALC_NO_MOVE(TYPE)		\
TYPE() = delete;						\
~TYPE() = delete;

namespace ALC {
	using std::string;

	class Debugger {
		ALC_NON_CONSTRUCTABLE(Debugger);
	public:

		static void Log(const string& msg, const string& file, size_t line);
		static void Trace(const string& msg, const string& file, size_t line);
		static void Warning(const string& msg, const string& file, size_t line);
		static void Error(const string& msg, const string& file, size_t line);
		static void FatalError(const string& msg, const string& file, size_t line);

	};

	namespace detail {
		using std::to_string;
		using glm::to_string;
	}

}

#define ALC_DEBUG_LOG(msg) \
	::ALC::Debugger::Log(msg, __FILE__, __LINE__)
#define ALC_DEBUG_TRACE(msg) \
	::ALC::Debugger::Trace(msg, __FILE__, __LINE__)
#define ALC_DEBUG_WARNING(msg) \
	::ALC::Debugger::Warning(msg, __FILE__, __LINE__)
#define ALC_DEBUG_ERROR(msg) \
	::ALC::Debugger::Error(msg, __FILE__, __LINE__)
#define ALC_DEBUG_FATAL_ERROR(msg) \
	::ALC::Debugger::FatalError(msg, __FILE__, __LINE__)

#define VTOS(value) \
	::ALC::detail::to_string(value)

#endif // !_CORE_DEBUGGER_HPP