//
// Created by oleg on 05.04.2020.
//

#ifndef HT10_SOCKET_H
#define HT10_SOCKET_H


#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

/* порты [0,1023] - системные, [1024,65535] доступны пользователю */
#define PORT  3490
/* размер очереди [listen()] входящих соединений ожидающих приема */
#define QUEUE 5

class Socket;

#endif //HT10_SOCKET_H
