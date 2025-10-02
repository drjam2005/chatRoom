#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <raylib.h>
#include <button.h>

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

class messageGUI {
	char user[1024] = {0};
	char message[1024] = {0};
public:
	messageGUI(char*, char*);
	void Render(int&, int, bool);
};


class messageField {
private:
	int MAX_LINES = 20;
	std::vector<_MESSAGE_PACKET> messages;
public:
	std::vector<_MESSAGE_PACKET> getMessages();
	messageField(int lines=20){
		MAX_LINES = lines;
		messages.reserve(MAX_LINES);
	}
	
	void AddMessage(_MESSAGE_PACKET);
};

class ClientUI {
	private:
		Client user;
		std::vector<Client> users;

		int socket_fd;
		messageBox userMsg;
	public:
		messageField messages;
		ClientUI(Client, int);
		Button b1;
		void Render();
		void parseChar();
		void parseKey();
		void AddMSG(_MESSAGE_PACKET);
		void sendMessage(_MESSAGE_PACKET);
		void UpdateUserList(Client, PACKET_TYPE);

		void Update();
};
