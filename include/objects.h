struct Client;
struct Message;

struct Client {
	int client_fd = 0;
	char username[256] = {0};
};

struct Message {
	char message[1024] = {0};
};
