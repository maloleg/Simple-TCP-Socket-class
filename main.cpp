#ifndef MAIN_CPP
#define MAIN_CPP


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
private:
    int sockfd;

protected:
    bool good;
    int sock = sockfd;

public:

    int DGetter(){return sockfd;}

    void DSetter(int s){sockfd = s;}

    Socket& Read(char* Text, std::streamsize TextSize){
        do
        {
            char data[1];
            int ret;
            Text = "";
//
            memset(data, 0x00, 1);

            /* чтение данных */
            for (int i = 0; i < TextSize; i++) {
                if ((ret = recv(sockfd, data, 1, 0)) == -1) {
                    perror("recv()");
                    break;
                }
                else{
                    Text += data[0];
                }
            }
        }
        while(0);
        return dynamic_cast<Socket&> (*this);
    }
    Socket& Write(const char* a, std::streamsize b){
            /* отправка данных по установленному каналу связи */
            for (int i = 0; i < b; i++) {
                if (send(sockfd, reinterpret_cast<const void *>(a[i]), 1, 0) == -1) {
                    perror("send()");
                }
            }
            return dynamic_cast<Socket&> (*this);
    }
    friend Socket& operator >> (Socket&, std::string&);

    friend Socket& operator << (Socket&, std::string&);

    void Shutdown(){
        if (shutdown(sockfd, SHUT_RDWR) == -1){
            perror("shutdown()");
        }
    }
};

Socket& operator >> (Socket& sock, std::string& Text){
    char *cstr = new char[Text.length() + 1];
    strcpy(cstr, Text.c_str());
// do stuff

    sock.Read(cstr, Text.length() + 1);
    delete [] cstr;
    return sock;

}

Socket& operator << (Socket& sock, std::string& TextStream){
    sock.Write(TextStream.c_str(), TextStream.size());
    return sock;
}

class ClientSocket : Socket{
private:
    struct sockaddr_in peer;
public:

    ClientSocket(int SockD, int port, char* ip){
        DSetter(SockD);

        memset(&peer, 0x00, sizeof(struct sockaddr_in));
        peer.sin_family      = AF_INET;
        peer.sin_port        = htons(port);
        peer.sin_addr.s_addr = inet_addr(ip);
    }

    ClientSocket(){
        memset(&peer, 0x00, sizeof(struct sockaddr_in));
        peer.sin_family      = AF_INET;
        peer.sin_port        = htons(PORT);
        peer.sin_addr.s_addr = inet_addr("127.0.0.1");
    }

    void Connect(){
        if (connect(DGetter(), (struct sockaddr*)&peer, sizeof(struct sockaddr_in)) == -1){
            perror("connect()");
            exit(1);
        }
    }
};

class ServerSocket : public Socket{
private:
    struct sockaddr_in self;
public:
    void Bind(){
        if(bind(DGetter(), (struct sockaddr*)&self, sizeof(struct sockaddr_in)) == -1){
            perror("bind()");
            exit(1);
        }
    }
    void Listen(){
        if(listen(DGetter(), QUEUE) == -1){
            perror("listen()");
            exit(1);
        }
    }
    ClientSocket Accept(){
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int cli_sock;

        if((cli_sock = accept(DGetter(), (struct sockaddr*)&self, &addrlen)) == -1)
        {
            perror("accept()");
            exit(1);
        }

        return ClientSocket(cli_sock, PORT, "127.0.0.1");

    }
};

int main() {
    //std::cout << "Hello, World!" << std::endl;
    ClientSocket a;
    ServerSocket b;

    a.Connect();

    while (1) {

        a.Write("Hello world!", 12);
    }

    return 0;
}
#endif //MAIN_CPP