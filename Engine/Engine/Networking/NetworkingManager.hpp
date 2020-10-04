#ifndef _NETWORKING_MANAGER_HPP
#define _NETWORKING_MANAGER_HPP
#include <thread>
#include <future>
#include "../General/Macros.hpp"

enum class EnvironmentType : unsigned char {
	// invalid environment
	Invalid,
	// if this program is a server
	Server,
	// if this program is a client
	Client
};

enum class ConnectionStatus : unsigned char {
	// if currently no connection
	NotActive,
	// if currently connected
	Active,
	// if the connection was made successfully
	Successful,
	// if the connection has failed
	Failed
};

struct ConnectionState {

	EnvironmentType type;
	ConnectionStatus status;

private:

	ConnectionState() : type(EnvironmentType::Invalid), status(ConnectionStatus::NotActive) { }
	ConnectionState(EnvironmentType type_, ConnectionStatus status_)
		: type(type_), status(status_) { }

	// checks
	bool IsValid() const { return type != EnvironmentType::Invalid; }
	operator bool() const { return type != EnvironmentType::Invalid; }

	// getters and setters
	EnvironmentType GetEnvType() const { return type; }
	ConnectionStatus GetStatus() const { return status; }

};

class NetworkingManager {
	NON_CONSTRUCTABLE(NetworkingManager);

	static std::thread srThread; // thread used for communication
	static std::future<ConnectionState> stateFuture;
	static ConnectionState state;

public:

	// events
	static void Init();
	static void Exit();

	// used to start end and use a networking connection
	static void CreateEnvironment(bool isHost);
	static void CloseEnvironment();

private:

	static void TryConnect();

};

#endif // !_NETWORKING_MANAGER_HPP