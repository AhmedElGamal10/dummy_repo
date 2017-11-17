/*
** client.cpp
*/


#include "functions.h"

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    // printf("argc: %d", argc);
    
    if (argc != 2)
    {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
// loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    printf("%d\n",sockfd );
    char* buffer = "msg";
    // Get the requests.

    cout << "size : ";
    vector<string> result = parse_file("read.txt");

    cout << result.size() <<endl;

    for (int i = 0; i < result.size(); i++) {
        cout << result.at(i) << endl;

        string request = get_request(result.at(i));
        std::vector<std::string> requestVector = parse_request(request);
        string fileType = getFileType(requestVector);

        cout << request << endl;


        if (send(sockfd, request.c_str(), request.size(), 0) == -1) perror("send");
        cout << "after send" << endl;
        // check if get or post.
        if (request.find(GET) != string::npos) { // get
            // receive file (download).
            cout << "fileType " << fileType << endl;
            cout << "before receive" << endl;
            
            char buffer[MAXDATASIZE];
            // char* buffer;
            if(fileType.compare("txt") == 0){
                
            }

            int result = recv(sockfd,buffer, 255, 0);
            if (result <= -1) {
                perror("recv");
                exit(1);
            }
            cout << "after receive" << endl;
            cout << "respond is: " << buffer << endl; 
        } else { // post
            // wait for OK message then sent file according to file type.
            char* buffer;    
            if (recv(sockfd, buffer, 255, 0) <= -1) {

            }

        }

        cout << endl;
    }
    

     
    /*if (send(sockfd, "hamada!", 13, 0) == -1) perror("send");
    printf("client: received '%s'\n",buf);*/
    close(sockfd);
    return 0;
}