#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include "../GeneralMacros.hpp"
#include "../GeneralTypes.hpp"

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

#define LOG(msg) Debugger::Get()->Log(msg, __FILE__, __LINE__)
#define TRACE(msg) Debugger::Get()->Trace(msg, __FILE__, __LINE__)
#define WARNING(msg) Debugger::Get()->Warning(msg, __FILE__, __LINE__)
#define ERROR(msg) Debugger::Get()->Error(msg, __FILE__, __LINE__)
#define FATAL_ERROR(msg) Debugger::Get()->FatalError(msg, __FILE__, __LINE__)

#endif // !_CORE_DEBUGGER_HPP