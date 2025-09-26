#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>

#include <raylib.h>

#include <objects.h>
#include <packets.h>

void consoleInput(int socket_fd){
	while (true){
		char message[1024] = {0};
		std::cin.getline(message, sizeof(message));

		PACKET_TYPE packet_type = MESSAGE;
		_MESSAGE_PACKET packet;
		strncpy(packet.message, message, sizeof(packet.message));
		send(socket_fd, &packet_type, sizeof(PACKET_TYPE), 0);
		send(socket_fd, &packet, sizeof(packet), 0);
	};
}

void handleIncomingPackets(int socket_fd){
	while (true){
		PACKET_TYPE pt;
		int bytes = recv(socket_fd, &pt, sizeof(PACKET_TYPE), 0);
		if(bytes <= 0){
			std::cout << "Server stoppeed..." << std::endl;
			return;
		}
		if(pt == USER_JOIN){
			Client newClient;
			recv(socket_fd, &newClient, sizeof(Client), 0);
			std::cout << "\nNew client: " << newClient.client_fd << " : " << newClient.username << std::endl;
		}
		
		if(pt == USER_EXISTING){
			Client existingClient;
			recv(socket_fd, &existingClient, sizeof(Client), 0);
			std::cout << "\nExisting client: " << existingClient.client_fd << " : " << existingClient.username << std::endl;
		}

		if(pt == USER_LEAVE){
			Client leftClient;
			recv(socket_fd, &leftClient, sizeof(Client), 0);
			std::cout << leftClient.username << " left..." << std::endl;
		}

		if(pt == MESSAGE){
			_MESSAGE_PACKET msg; 
			int rec = recv(socket_fd, &msg, sizeof(_MESSAGE_PACKET), 0); if(rec <= 0) return;
			std::cout << msg.client.username << ": " << msg.message << std::endl;
		}

	}
}

int main(){ 
	int port; char name[256] = {0};
	std::cout << "Username: "; std::cin >> name;
	std::cout << "Port to join: "; std::cin >> port;
	std::cin.ignore();

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0) return -1;
	
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

	int connect_code = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));
	if(connect_code <= -1){
		std::cerr << "Could not connect..." << std::endl;
		return -2;
	}

	// send(socket_fd, (PACKET_TYPE*)(JOIN), sizeof(PACKET_TYPE), 0);
	send(socket_fd, name, sizeof(name), 0);
	std::thread test(consoleInput, socket_fd);
	std::thread handlePackets(handleIncomingPackets, socket_fd);
	test.detach();
	handlePackets.detach();
	

	// window rendering
	SetTraceLogLevel(LOG_ERROR); 
	InitWindow(100, 100, "window");
	SetTargetFPS(30);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(GRAY);

	

		EndDrawing();
	}
	while(true)continue;
	return 0; 
}
