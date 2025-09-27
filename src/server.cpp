#include <iostream>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>

#include <objects.h>
#include <packets.h>

std::vector<Client> clients;

void handleClientPackets(Client client){
	while(true){
		PACKET_TYPE packet_type;
		int rec = recv(client.client_fd, &packet_type, sizeof(packet_type), 0);

		if(rec == 0){
			std::cout << client.username << " left..." << std::endl;
			for(Client& clnt : clients){
				if(clnt.client_fd == client.client_fd) continue;
				PACKET_TYPE pt = USER_LEAVE;
				send(clnt.client_fd, &pt, sizeof(PACKET_TYPE), 0);
				send(clnt.client_fd, &client, sizeof(Client), 0);
			}
			return;
		}

		if(packet_type == MESSAGE){
			_MESSAGE_PACKET packet; 
			rec = recv(client.client_fd, &packet, sizeof(packet), 0);
			packet.client = client;
			std::cout << packet.client.username << " said: " << packet.message << std::endl;

			for(Client& clnt : clients){
				if(clnt.client_fd == client.client_fd) continue;
				PACKET_TYPE pt = MESSAGE;
				send(clnt.client_fd, &pt, sizeof(PACKET_TYPE), 0);
				send(clnt.client_fd, &packet, sizeof(_MESSAGE_PACKET), 0);
			}
		}
	}
}

void handleJoin(int socket_fd){
	while(true){
		Client client = {0};
		int client_fd = accept(socket_fd, 0, 0);
		int theID = client_fd;
		int netID = htonl(theID);
		send(client_fd, &netID, sizeof(netID), 0);

		char username[256] = {0};
		int rec = recv(client_fd, &username, sizeof(username), 0);
		if(rec <= 0){
			std::cout << "what..." << std::endl;
			close(client_fd);
			return;
		}

		client.client_fd = client_fd;
		strncpy(client.username, username, sizeof(username));
		std::cout << "Client joined: " << client.username << std::endl;

		for(Client& clnt : clients){
			PACKET_TYPE type = USER_JOIN;
			send(clnt.client_fd, &type, sizeof(PACKET_TYPE), 0);
			send(clnt.client_fd, &client, sizeof(Client), 0);

			type = USER_EXISTING;
			send(client.client_fd, &type, sizeof(PACKET_TYPE), 0);
			send(client.client_fd, &clnt, sizeof(Client), 0);
		}

		clients.push_back(client);

		std::thread handle_client(handleClientPackets, client);
		handle_client.detach();
	}
}

int main(int argc, char** argv){
	if(argc != 2){ 
		std::cerr << "1 more argument required..." << std::endl;
		std::cerr << "Usage: ./server <PORT_NUMBER>" << std::endl;
		std::cerr << "example: ./server 5050" << std::endl;
		return -1;
	}

	std::string arg = argv[1];
	if(arg == "--help" || arg == "-h"){
		std::cerr << "Usage: ./server <PORT_NUMBER>" << std::endl;
		std::cerr << "example: ./server 5050" << std::endl;
		return -2;
	}

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0) return -3;
	
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(std::atoi(argv[1]));
	inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

	int bind_code = bind(socket_fd, (struct sockaddr*)&address, sizeof(address));
	if(bind_code <= -1){
		std::cerr << "Could not bind..." << std::endl;
		return -4;
	}

	int code = listen(socket_fd, 32);
	if(code < 0) return -3;
	std::cout << "Listening to port: " << argv[1] << "..." << std::endl;
	std::thread handle_join(handleJoin, socket_fd);
	handle_join.detach();

	while (true) continue;
	return 0;
}
