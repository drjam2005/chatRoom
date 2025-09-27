#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <raylib.h>

// forward dec
struct _MESSAGE_PACKET;
#ifndef PACKTYPEDEF
#define PACKTYPEDEF
enum PACKET_TYPE {
	// user joining/leaving
	USER_JOIN,
	USER_EXISTING,
	USER_LEAVE,

	// message handling
	MESSAGE,
};
#endif

struct Client;
struct Client {
	int client_fd = 0;
	char username[256] = {0};
	
    bool operator==(const Client& other) {
        return client_fd == other.client_fd;
    }
};

class messageBox {
	private:
		char message[1024] = {0};
		int index = 0;
	public:
		void ParseKey(int);
		void AddChar(char chr);
		char* getMsg();
};


struct messageData {
	Client user;
	char message[1024] = {0};
};

class messageField {
private:
	int MAX_LINES = 20;
	std::vector<messageData> messages;
public:
	std::vector<messageData> getMessages();
	messageField(int lines=20){
		MAX_LINES = lines;
		messages.reserve(MAX_LINES);
	}
	
	void AddMessage(_MESSAGE_PACKET msg);
};

class ClientUI {
	private:
		Client user;
		std::vector<Client> users;

		int socket_fd;
		messageBox userMsg;
		messageField messages;
	public:
		ClientUI(Client, int);
		void Render();
		void parseChar();
		void parseKey();
		void AddMSG(_MESSAGE_PACKET);
		void sendMessage(_MESSAGE_PACKET);
		void UpdateUserList(Client, PACKET_TYPE);

		void Update();
};
