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
/* размер очереди [listen()] входящих соединений ожидающих приема */
#define QUEUE 5


int main()
{
    /* дескрипторы для ожидиния и приема входящих соединений */
    int svr_sock;
    int cli_sock;
    /* адрес для ожидания и адрес принятого входящего соединения */
    struct sockaddr_in svr_addr;
    struct sockaddr_in cli_addr;

    /* socket(семейство протоколов, тип сокета, номер протокола) */
    if( (svr_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("socket()");
        exit(1);
    }

    /* задание адреса ожидания входящих соединений */
    memset(&svr_addr, 0x00, sizeof(struct sockaddr_in));
    svr_addr.sin_family      = AF_INET;
    svr_addr.sin_port        = htons(PORT);
    svr_addr.sin_addr.s_addr = INADDR_ANY;

    /* привязка дескриптора к адресу */
    if( bind( svr_sock,
              (struct sockaddr*)&svr_addr, sizeof(struct sockaddr_in)
    ) == -1 )
    {
        perror("bind()");
        exit(1);
    }

    /* задание очереди входящих соединений */
    if( listen(svr_sock, QUEUE) == -1 )
    {
        perror("listen()");
        exit(1);
    }

    /* основной цикл обработки входящих соединений */
    while(1)
    {
        socklen_t addrlen = sizeof(struct sockaddr_in);

        /* прием входящего соединения и перевод его на новый дескриптор */
        if( (cli_sock=accept(svr_sock, (struct sockaddr*)&cli_addr, &addrlen)
            ) == -1)
        {
            perror("accept()");
            continue;
        }

        /* вывод информации о принятом входящем соединении */
        printf("SERVER: connection from %s\n",
               inet_ntoa(cli_addr.sin_addr)  );

        /* отправка данных по установленному каналу связи */
        if( send(cli_sock, "Hello, World!\n", 14, 0) == -1 )
        {
            perror("send()");
        }

        /* разрыв соединения (запрет на чтение и запись) */
        if( shutdown(cli_sock, SHUT_RDWR) == -1 )
        {
            perror("shutdown()");
        }
    }

    /* освобождение ресурсов дескриптора сокета */
    if( shutdown(svr_sock, SHUT_RDWR) == -1 )
    {
        perror("shutdown()");
    }

    return 0;
}