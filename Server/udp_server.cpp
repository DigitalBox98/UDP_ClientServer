#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    int sockfd;
    struct sockaddr_in servaddr{}, cliaddr{};
    char buffer[BUFFER_SIZE];

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Initialize server address
    memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket with address
    if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    std::vector<struct sockaddr_in> clients;

    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';

        // Print received message
        std::cout << "Message from " << inet_ntoa(cliaddr.sin_addr) << ":" << ntohs(cliaddr.sin_port) << " - " << buffer << std::endl;

        // Add client to list if not already present
        bool clientExists = false;
        for (const auto &client : clients) {
            if (client.sin_addr.s_addr == cliaddr.sin_addr.s_addr && client.sin_port == cliaddr.sin_port) {
                clientExists = true;
                break;
            }
        }
        if (!clientExists) {
            clients.push_back(cliaddr);
        }

        // Broadcast message to all clients
        for (const auto &client : clients) {
            sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &client, sizeof(client));
        }
    }

    close(sockfd);
    return 0;
}

