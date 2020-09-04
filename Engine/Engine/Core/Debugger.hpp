#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"

class Debugger {
	SINGLETON(Debugger);

	string logFilepath;

	Debugger();
	~Debugger();

public:

	void Log(const string& msg, const string& file, size_t line);
	void Trace(const string& msg, const string& file, size_t line);
	void Warning(const string& msg, const string& file, size_t line);
	void Error(const string& msg, const string& file, size_t line);
	void FatalError(const string& msg, const string& file, size_t line);

};

#define DEBUG_LOG(msg) Debugger::Get()->Log(msg, __FILE__, __LINE__)
#define DEBUG_TRACE(msg) Debugger::Get()->Trace(msg, __FILE__, __LINE__)
#define DEBUG_WARNING(msg) Debugger::Get()->Warning(msg, __FILE__, __LINE__)
#define DEBUG_ERROR(msg) Debugger::Get()->Error(msg, __FILE__, __LINE__)
#define DEBUG_FATAL_ERROR(msg) Debugger::Get()->FatalError(msg, __FILE__, __LINE__)

#define VTOS(value) to_string(value)

inline const string& to_string(const string& str) {
	return str;
}

#endif // !_CORE_DEBUGGER_HPP