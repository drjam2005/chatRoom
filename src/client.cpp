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

void handleIncomingPackets(int socket_fd, ClientUI& ui){
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
			ui.UpdateUserList(newClient, USER_JOIN);
			std::cout << "\nNew client: " << newClient.client_fd << " : " << newClient.username << std::endl;
		}
		
		if(pt == USER_EXISTING){
			Client existingClient;
			recv(socket_fd, &existingClient, sizeof(Client), 0);
			ui.UpdateUserList(existingClient, USER_EXISTING);
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
			ui.AddMSG(msg);
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

	Client me;
	strncpy(me.username, name, sizeof(me.username));

	int netID;
	recv(socket_fd, &netID, sizeof(netID), 0);
	me.client_fd = ntohl(netID);
	std::cout << ntohl(netID) << " : " << me.client_fd << std::endl;


	ClientUI ui(me, socket_fd);
	send(socket_fd, name, sizeof(name), 0);
	std::thread handlePackets(handleIncomingPackets, socket_fd, std::ref(ui));
	handlePackets.detach();
	
	// window rendering
	SetTraceLogLevel(LOG_ERROR); 
	InitWindow(800, 600, "window");
	SetTargetFPS(30);
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(GRAY);

		ui.Update();

		ui.parseChar();
		ui.parseKey();

		ui.Render();

		EndDrawing();
	}
	while(true)continue;
	return 0; 
}
