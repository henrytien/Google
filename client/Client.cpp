//
// Created by henry on 1/18/2020.
//

#include "Client.h"

void do_service(int sockfd) {
    char recvbuf[1 << 12] = {0}; //4k
    char sendbuf[1 << 12] = {0};
    while (true) {
        memset(recvbuf, 0, sizeof(recvbuf));
        int nread = read(sockfd, recvbuf, sizeof(recvbuf));
        if (nread == Error) {
            if (errno == EINTR)
                continue;
            ERR_EXIT("read");
        } else if (nread == 0) {
            LogError("server close!");
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        LogInfo(" receve message = " + string(recvbuf) + " ");
        printf("\n");
        memset(sendbuf, 0, sizeof(sendbuf));
        fgets(sendbuf, sizeof(sendbuf), stdin);
        write(sockfd, sendbuf, strlen(sendbuf));
    }
}


//
int main(int argc, const char *argv[]) {
    if (argc != 2) {
        LogError("input my.conf path error!");
        return -1;
    }

    char tmp[10] = {0};
    char ip[16] = {0};
    int port;
    FILE *fp = fopen(argv[1], "rb");
    if (NULL == fp) {
        LogError("fopen error!");
    }
    fscanf(fp, "%s%s", tmp, ip);
    fscanf(fp, "%s%d", tmp, &port);
    fclose(fp);
    int peerfd = socket(PF_INET, SOCK_STREAM, 0);
    if (Error == peerfd)
        ERR_EXIT("socket");
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    socklen_t len = sizeof(addr);
    if (connect(peerfd, (struct sockaddr *) &addr, len) == Error)
        ERR_EXIT("Connect fail");
#if 0
    int peerfd = socket(PF_INET,SOCK_STREAM,0);
    if(Error == peerfd)
        ERR_EXIT("socket");
    Socket client(peerfd);
#endif
    do_service(peerfd);
    return 0;
}

