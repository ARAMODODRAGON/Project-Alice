#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"
#include <mutex>

class Debugger {
	PRIVATE_SINGLETON(Debugger);

	string logFilepath;
	
	// multithreading safe
	std::mutex lock;

	Debugger();
	~Debugger();

public:

	static void Log(const string& msg, const string& file, size_t line);
	static void Trace(const string& msg, const string& file, size_t line);
	static void Warning(const string& msg, const string& file, size_t line);
	static void Error(const string& msg, const string& file, size_t line);
	static void FatalError(const string& msg, const string& file, size_t line);

};

#define DEBUG_LOG(msg) Debugger::Log(msg, __FILE__, __LINE__)
#define DEBUG_TRACE(msg) Debugger::Trace(msg, __FILE__, __LINE__)
#define DEBUG_WARNING(msg) Debugger::Warning(msg, __FILE__, __LINE__)
#define DEBUG_ERROR(msg) Debugger::Error(msg, __FILE__, __LINE__)
#define DEBUG_FATAL_ERROR(msg) Debugger::FatalError(msg, __FILE__, __LINE__)

#define VTOS(value) to_string(value)

inline const string& to_string(const string& str) {
	return str;
}

#endif // !_CORE_DEBUGGER_HPP