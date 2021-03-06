#ifndef MAIN_CPP
#define MAIN_CPP

#include "Socket.h"

class Socket{
private:
    int sockfd;
protected:
    bool good;
public:
    int DGetter();

    void DSetter(int s);

    Socket& Read(char* Text, std::streamsize TextSize);

    Socket& Write(const char* a, std::streamsize b);

    friend Socket& operator >> (Socket&, std::string&);

    friend Socket& operator << (Socket&, std::string&);

    void Shutdown();

    virtual ~Socket() = 0;
};

Socket& operator >> (Socket& sock, std::string& Text){
    char *cstr = new char[Text.length() + 1];
    strcpy(cstr, Text.c_str());

    sock.Read(cstr, Text.length() + 1);
    delete [] cstr;
    return sock;

}

Socket& operator << (Socket& sock, std::string& TextStream){
    sock.Write(TextStream.c_str(), TextStream.size());
    return sock;
}

Socket &Socket::Read(char *Text, std::streamsize TextSize) {
    do
    {
        char data[1];
        int ret;
        Text = "";

        memset(data, 0x00, 1);

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

Socket &Socket::Write(const char *a, std::streamsize b) {
    for (int i = 0; i < b; i++) {
        if (send(sockfd, reinterpret_cast<const void *>(a[i]), 1, 0) == -1) {
            perror("send()");
        }
    }
    return dynamic_cast<Socket&> (*this);
}

Socket::~Socket() {}

void Socket::Shutdown() {
    if (shutdown(sockfd, SHUT_RDWR) == -1){
        perror("shutdown()");
    }
}

void Socket::DSetter(int s) {sockfd = s;}

int Socket::DGetter() {return sockfd;}

class ClientSocket : Socket{
private:
    struct sockaddr_in peer;
public:
    ClientSocket(int port, char* ip);

    ClientSocket();

    void Connect();

};



ClientSocket::ClientSocket(int port, char *ip) {

    memset(&peer, 0x00, sizeof(struct sockaddr_in));
    peer.sin_family      = AF_INET;
    peer.sin_port        = htons(port);
    peer.sin_addr.s_addr = inet_addr(ip);
}

ClientSocket::ClientSocket() {
    memset(&peer, 0x00, sizeof(struct sockaddr_in));
    peer.sin_family      = AF_INET;
    peer.sin_port        = htons(PORT);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void ClientSocket::Connect() {
    if (connect(DGetter(), (struct sockaddr*)&peer, sizeof(struct sockaddr_in)) == -1){
        perror("connect()");
        exit(1);
    }
}


class ServerSocket : Socket{
private:
    struct sockaddr_in self;
public:
    ServerSocket(int port, char *ip);

    ServerSocket();

    void Bind();

    void Listen();

    ClientSocket Accept();
};

ServerSocket::ServerSocket(int port, char *ip) {

    memset(&self, 0x00, sizeof(struct sockaddr_in));
    self.sin_family      = AF_INET;
    self.sin_port        = htons(port);
    self.sin_addr.s_addr = inet_addr(ip);
}

ServerSocket::ServerSocket() {
    memset(&self, 0x00, sizeof(struct sockaddr_in));
    self.sin_family      = AF_INET;
    self.sin_port        = htons(PORT);
    self.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void ServerSocket::Bind() {
    if(bind(DGetter(), (struct sockaddr*)&self, sizeof(struct sockaddr_in)) == -1){
        perror("bind()");
        exit(1);
    }
}

void ServerSocket::Listen() {
    if(listen(DGetter(), QUEUE) == -1){
        perror("listen()");
        exit(1);
    }
}

ClientSocket ServerSocket::Accept() {
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int cli_sock;

    if((cli_sock = accept(DGetter(), (struct sockaddr*)&self, &addrlen)) == -1)
    {
        perror("accept()");
        exit(1);
    }

    return ClientSocket(PORT, INADDR_ANY);

}

int main() {
    ClientSocket a(3490, "194.58.102.54");

    //a.Connect();

    std::cout << "MEW MEW ATTENTION MEW MEW\n"
                 "I really didn't get, why we should return reference to a class with methods Write and Read.\n"
                 "And if we do so, how we should use it?\nAlso how should we do an abstract class without making any method pure virtual?\n"
                 "I think i did one (a destructor), but it all seems like a linguistic crutch :(\n"
                 "MEW MEW ATTENTION MEW MEW";

    return 0;
}
#endif //MAIN_CPP