/* для printf() */
#include <stdio.h>
/* для exit()   */
#include <stdlib.h>
/* для memset() */
#include <string.h>

/* для использования системных вызовов:
 *   socket()  - создание файловолго дескриптора сокета
 *   bind()    - (сервер) привязка дескриптора к адресу
 *   listen()  - (сервер) задание очереди входящих соединенией
 *   accept()  - (сервер) прием входящего соединения из очереди
 *   connect() - (клиент) установление исходящего соединения
 */
#include <sys/types.h>
#include <sys/socket.h>

/* для функций inet_ntoa() и htons() - преобразующих порядок байт */
#include <netinet/in.h>
#include <arpa/inet.h>


/* порты [0,1023] - системные, [1024,65535] доступны пользователю */
#define PORT  3490
/* размер массива для чтения данных */
#define SIZE  1024


int main()
{
    /* дескриптор для исходящего соединения */
    int cli_sock;
    /* адрес для установления исходящего соединения */
    struct sockaddr_in svr_addr;

    /* socket(семейство протоколов, тип сокета, номер протокола) */
    if( (cli_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("socket()");
        exit(1);
    }

    /* задание адреса для установления исходящего соединения */
    memset(&svr_addr, 0x00, sizeof(struct sockaddr_in));
    svr_addr.sin_family      = AF_INET;
    svr_addr.sin_port        = htons(PORT);
    svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* создание исходящего соединения */
    if( connect( cli_sock, (struct sockaddr*)&svr_addr, sizeof(struct sockaddr_in)) == -1 )
    {
        perror("connect()");
        exit(1);
    }

    /* попытка чтения данных из канала */
    do
    {
        /* массив для чтения данных */
        char data[SIZE];
        int  ret;

        memset(data,0x00,SIZE);

        /* чтение данных */
        if( (ret=recv(cli_sock, data, SIZE-1, 0)) == -1 )
        {
            perror("recv()");
            break;
        }

        printf("CLIENT: received %d bytes of data=%s\n", ret, data);

    }
    while(0) ;

    /* разрыв соединения (запрет на чтение и запись) */
    if( shutdown(cli_sock, SHUT_RDWR) == -1 )
    {
        perror("shutdown()");
    }

    return 0;
}