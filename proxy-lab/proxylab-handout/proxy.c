#include <stdio.h>
#include <csapp.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
void doit(int fd);
int read_requesthdrs(rio_t *rp, char* hdr);
void parse_url(char* uri, char* host, char* path, char* port_num);
int forward_request(int connfd, char* host, char* path, char* header, char* port);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);
void sigpipe_handler(int sig);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

int main(int argc, char** argv) {
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    if (listenfd < 0) {
        fprintf(stderr, "failed to open listening port");
        exit(2);
    }
    Signal(SIGPIPE, sigpipe_handler);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd) {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], hdr[MAXBUF];
    char host[MAXLINE], path[MAXLINE], port[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Proxy server does not implement this method");
        return;
    }
    strcpy(hdr, "");
    if (read_requesthdrs(&rio, hdr) < 0) {
        return;
    }

    /* Parse URI from GET request */
    strcpy(host, "");
    strcpy(path, "");
    strcpy(port, "");
    parse_url(uri, host, path, port);

    int status;
    if ((status = forward_request(fd, host, path, hdr, port)) < 0) {
        switch (status) {
            case -2:
                clienterror(fd, path, "502", "Bad Gateway", "");
                break;
            case -3:
                clienterror(fd, path, "503", "Gateway Time-out", "");
                break;
            default:
                break;
        }
    }

}

int read_requesthdrs(rio_t *rp, char* hdr)
{
    char buf[MAXLINE];

    if (Rio_readlineb(rp, buf, MAXLINE) == -1 && errno == ECONNRESET) {
        return -1;
    }

    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {
        if (!strstr(buf, "Proxy-Connection:") && !strstr(buf, "Connection:") &&
        !strstr(buf, "User-Agent:")) {
            strcat(hdr, buf);
        }
        if (Rio_readlineb(rp, buf, MAXLINE) == -1 && errno == ECONNRESET) {
            return -1;
        }
        printf("%s", buf);
    }
    return 0;
}

void parse_url(char* uri, char* host, char* path, char* port_num) {
    int offset = 7;

    // when uri does not include http, for example: localhost
    if (!strstr(uri, "http://")) {
        offset = 0;
    }
    uri += offset;
    char* ptr = uri;
    int hostname_length = 0;

    // when uri only includes hostname
    if (!strstr(uri, "/")) {
        strcpy(host, uri);
        hostname_length = strlen(host);
        strcpy(path, "/");
    }
    else {
        while (*(ptr + hostname_length) != '/' && *(ptr + hostname_length) != ':') {
            hostname_length += 1;
        }
        strncpy(host, uri, hostname_length);
        *(host + hostname_length) = '\0';
    }
    ptr += hostname_length;

    // set port number
    sprintf(port_num, "%d", 80);
    if (*ptr == ':') {
        int port;
        ptr += 1;
        port = strtol(ptr, &ptr, 10);
        sprintf(port_num, "%d", port);
    }

    // set file path
    if (*ptr == '/') {
        strcat(path, ptr);
    }
}

/* return -2 when connection with server aborted
 *        -3 when no connection with server was made
 */
int forward_request(int connfd, char* host, char* path, char* header, char* port) {
    rio_t rio;
    int clientfd = Open_clientfd(host, port);
    if (clientfd < 0) {
        // Send a message to client: all attempts of connection failed
        return -3;
    }
    Rio_readinitb(&rio, clientfd);
    char buf[MAXBUF];
    sprintf(buf, "GET %s HTTP/1.0\r\n", path);
    Rio_writen(clientfd, buf, strlen(buf));
    if (errno == EPIPE) {
        // Send a message to client: connection closed by server
        return -2;
    }

    if (!strstr(header, "Host:")) {
        sprintf(buf, "Host: %s\r\n", host);
    }
    strcat(buf, header);
    sprintf(buf, "%s%s", buf, user_agent_hdr);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sProxy-Connection: close\r\n\r\n", buf);
    printf("headers to send: %s", buf);
    Rio_writen(clientfd, buf, strlen(buf));
    if (errno == EPIPE) {
        // Send a message to client: connection closed by server
        return -2;
    }

    ssize_t rc;
    while ((rc = Rio_readnb(&rio, buf, RIO_BUFSIZE)) > 0) {
        Rio_writen(connfd, buf, rc);
        if (errno == EPIPE) {
            // Connection closed prematurely by client
            return 1;
        }
    }
    if (rc < 0 && errno == ECONNRESET) {
        return -2;
    }
    return 0;
}

void sigpipe_handler(int sig) {
    return;
}

void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Proxy Server Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Proxy Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}