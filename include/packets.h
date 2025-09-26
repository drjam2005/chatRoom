enum PACKET_TYPE {
	MESSAGE,
};

struct MESSAGE_PACKET {
	int client_fd = 0;
	char message[1024] = {0};
};
