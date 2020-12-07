#include "NetworkedLevel.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#include <queue>
#include "../BattleDemo/BattleDemoCamera.hpp"

namespace {

	SOCKET outsock = INVALID_SOCKET;
	sockaddr_in outaddr {};

	std::queue<json> recievedData;
	std::mutex dataLock;

	std::atomic_bool shouldStop = false;
	std::atomic_bool isA = false;
	std::thread reciveThread;

	void ReciveLoop() {
		DEBUG_LOG("running!");

		vector<char> buffer;
		buffer.reserve(1024);
		buffer.resize(1024, 0);

		// Create a socket, notice that it is a user datagram socket (UDP)
		SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

		// Create a server hint structure for the server
		sockaddr_in otherHint;
		otherHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
		otherHint.sin_family = AF_INET; // Address format is IPv4
		otherHint.sin_port = htons(54000); // Convert from little to big endian

		// Try and bind the socket to the IP and port
		if (bind(in, (sockaddr*)&otherHint, sizeof(otherHint)) == SOCKET_ERROR) {
			std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
			return;
		}

		while (shouldStop == false) {

			int bytesIn = recvfrom(in, buffer.data(), buffer.size(), 0, 0, 0);
			if (bytesIn == SOCKET_ERROR) {
				DEBUG_ERROR("Error receiving from client " + VTOS(WSAGetLastError()));
			}
			//if (bytesIn > buffer.size()) buffer.resize(bytesIn, 0);
			//recvfrom(sock, buffer.data(), buffer.size(), 0, 0, 0);

			json j;
			try { j = json::parse(buffer); }
			catch (const std::exception& e) { continue; }

			{
				std::lock_guard<std::mutex> guard(dataLock);
				recievedData.push(std::move(j));
			}

		}

		closesocket(in);
	}

}

void NetworkedLevel::Init() {
	//bool isHost;
	//if (const LobbyData* dat = NetworkManager::GetLobby()) {
	//	isHost = dat->thisIsOwner;
	//}
	//// not connected to server
	//else { Game::Get()->Quit(); return; }

	string msg;
	std::getline(std::cin, msg);
	if (msg == "A") isA = true;

	// start recieving thread only on B
	shouldStop = false;
	if (!isA) reciveThread = std::thread(ReciveLoop);

	// create the out socket
	if (isA) {
		outsock = socket(AF_INET, SOCK_DGRAM, 0);
		outaddr.sin_family = AF_INET; // AF_INET = IPv4 addresses
		outaddr.sin_port = htons(54000); // Little to big endian conversion
		inet_pton(AF_INET, "127.0.0.1", &outaddr.sin_addr); // Convert from string to byte array
	}

	ObjectFactory::Make<BattleDemoCamera>();
	thisPlayer = ObjectFactory::Make<NetPlayer>();
	thisPlayer->GetSprite()->LoadTexture("64by64texture");
	otherPlayer = ObjectFactory::Make<NetPlayer>();
	otherPlayer->GetSprite()->LoadTexture("64by64texture");

}

void NetworkedLevel::Update() {
	counter++;
	thisPlayer->DoLocalMovement();
}

void NetworkedLevel::LateUpdate() {
	// test send
	if (isA) {
		vec2 pos = thisPlayer->GetPosition();
		vec2 vel = thisPlayer->GetVelocity();
		json j = {
			{ "pos", {{ "x", pos.x }, { "y", pos.y }} },
			{ "vel", {{ "x", vel.x }, { "y", vel.y }} }
		};
		string msg = j.dump(-1);

		if (sendto(outsock, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&outaddr, sizeof(outaddr))
			== SOCKET_ERROR) {
			DEBUG_ERROR("That didn't work! " + VTOS(WSAGetLastError()));
		}
		if (Keyboard::GetKey(KeyCode::KeyF).Pressed()) {
		}
		if ((counter % 20) == 0) {

		}
	}
	else {
		std::queue<json> localqueue;
		if ((counter % 3) == 0) {
			std::lock_guard<std::mutex> guard(dataLock);
			std::swap(localqueue, recievedData);
		}

		while (!localqueue.empty()) {
			json& j = localqueue.front();

			try {
				vec2 pos;
				pos.x = j["pos"]["x"];
				pos.y = j["pos"]["y"];
				vec2 vel;
				vel.x = j["vel"]["x"];
				vel.y = j["vel"]["y"];
				otherPlayer->ReadInput(pos, vel);
			}
			catch (const json::out_of_range& e) {
				DEBUG_ERROR("out of range error? " + string(e.what()));
			}

			localqueue.pop();
		}
	}
}

void NetworkedLevel::Exit() {

	shouldStop = true;
	if (!isA) reciveThread.join();

	closesocket(outsock);

}

NetPlayer::NetPlayer() {

}

NetPlayer::~NetPlayer() {
}

void NetPlayer::DoLocalMovement() {
	const auto up = Keyboard::GetKey(KeyCode::ArrowUp);
	const auto down = Keyboard::GetKey(KeyCode::ArrowDown);
	const auto left = Keyboard::GetKey(KeyCode::ArrowLeft);
	const auto right = Keyboard::GetKey(KeyCode::ArrowRight);

	vec2 vel;
	if (up != down) {
		if (up) vel.y = 1;
		if (down) vel.y = -1;
	}
	if (right != left) {
		if (right) vel.x = 1;
		if (left) vel.x = -1;
	}

	SetVelocity(vel * 100.0f);
	//DEBUG_LOG(VTOS(vel) + "," + VTOS(GetPosition()));
}

void NetPlayer::ReadInput(vec2 position, vec2 velocity) {
	SetPosition(position);
	SetVelocity(velocity);
}
