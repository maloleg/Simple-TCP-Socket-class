#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024
/* порты [0,1023] - системные, [1024,65535] доступны пользователю */
#define PORT  3490
/* размер очереди [listen()] входящих соединений ожидающих приема */
#define QUEUE 5

class Socket{
protected:
    bool good;
    int sockfd;
public:
    Socket& Read(char*, std::streamsize){
        do
        {
            char data[SIZE];
            int ret;

            memset(data, 0x00, SIZE);

            /* чтение данных */
            if ((ret = recv(sockfd, data, SIZE-1, 0)) == -1){
                perror("recv()");
                break;
            }

            printf("CLIENT: received %d bytes of data=%s\n", ret, data);

        }
        while(0);
    }
    Socket& Write(const char*, std::streamsize);
    Socket& operator >> (Socket&, std::string&);
    Socket& operator << (Socket&, const std::string&);
    void Shutdown(){
        if (shutdown(sockfd, SHUT_RDWR) == -1){
            perror("shutdown()");
        }
    }
};

class ClientSocket : Socket{
private:
    struct sockaddr_in peer;
public:
    ClientSocket(){
        if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket()");
            exit(1);
        }

        memset(&peer, 0x00, sizeof(struct sockaddr_in));
        peer.sin_family      = AF_INET;
        peer.sin_port        = htons(PORT);
        peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    }
    
    void Connect(){
        if (connect( sockfd, (struct sockaddr*)&peer, sizeof(struct sockaddr_in)) == -1){
            perror("connect()");
            exit(1);
        }
    }
};

class ServerSocket : Socket{
private:
    struct sockaddr_in self;
public:
    void Bind(){
        if( bind( sockfd, (struct sockaddr*)&self, sizeof(struct sockaddr_in)) == -1){
            perror("bind()");
            exit(1);
        }
    }
    void Listen(){
        if( listen(sockfd, QUEUE) == -1 ){
            perror("listen()");
            exit(1);
        }
    }
    ClientSocket Accept();

};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
