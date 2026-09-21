#include <string.h>
#include <sys/param.h>
#include <stdio.h>

#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define PORT 3333

static const char *TAG = "udp_server";

static void udp_server_task(void)
{
    char rx_buffer[128];
    char addr_str[128];

    while (1) {

            struct sockaddr_in dest_addr;
            dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_port = htons(PORT);

        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock < 0) {
            printf("%s: Unable to create socket: errno %d\n",TAG, errno);
            break;
        }
        printf("%s: Socket created\n",TAG);


        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            printf("%s: Socket unable to bind: errno %d\n",TAG, errno);
        }
        printf("%s: Socket bound, port %d\n",TAG, PORT);

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);


        while (1) {
            printf("%s: Waiting for data\n", TAG);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
            // Error occurred during receiving
            if (len < 0) {
                printf("%s: recvfrom failed: errno %d\n",TAG, errno);
                break;
            }
            // Data received
            else {
                // Get the sender's ip address as string
                strcpy(addr_str, inet_ntoa(((struct sockaddr_in *)&source_addr)->sin_addr));

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                printf("%s: Received %d bytes from %s:\n",TAG, len, addr_str);
                printf("%s: %s\n",TAG, rx_buffer);

                }
            }

        if (sock != -1) {
            printf("%s: Shutting down socket and restarting...\n",TAG);
            shutdown(sock, 0);
            close(sock);
        }
    }
}

int main(void)
{

  udp_server_task();
  return 0;
}
