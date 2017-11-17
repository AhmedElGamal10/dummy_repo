/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define HTTP "HTTP/1.1"
#define IMAGE "image"
#define HTML "html"
#define TXT "txt"
#define HOST "Host:"
#define NEWLINE "\n"
#define SPACE " "

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Parsing the command files.
vector<string> parse_file(string file_name) {
    vector<string> result;
    ifstream inFile;
    inFile.open(file_name.c_str());

    if (!inFile.is_open()) {
        cerr << "Unable to open file datafile.txt";
        // exit(1);   // call system to stop
    }
    string x;
    while (std::getline(inFile, x)) {
        if (x.compare(NEWLINE) == 0) continue; // not working :V.
        result.push_back(x);
    }
    inFile.close();
    return result;
}


vector<string> split (string s, string delimiter ) {
    size_t pos = 0;
    vector<string> result;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        result.push_back(token);
    }
    result.push_back(s); //check that
    return result;
}


string get_request(string request) {
    string result;
    string file_type;
    vector<string> client_request = split(request, SPACE);
    for (int i = 0; i < client_request.size() - 1; i++) { // size 1 => port number not used
        if (i == 2) {
            result.append(HTTP);
            result += NEWLINE;
            result.append(HOST);
            result += SPACE;
        }
        result.append(client_request.at(i));
        result += SPACE;
    }
    result += NEWLINE;
    return result;
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

    cout << "size: ";

    vector<string> result = parse_file("read.txt");
    cout << result.size() <<endl;

    for (int i = 0; i < result.size(); i++) {
        cout << result.at(i) << endl;
        string request = get_request(result.at(i));
        cout << request << endl;
        if (send(sockfd, request.c_str(), request.size(), 0) == -1) perror("send");
        cout << endl;
    }

    /*if (send(sockfd, "hamada!", 13, 0) == -1) perror("send");
    printf("client: received '%s'\n",buf);*/
    close(sockfd);
    return 0;
}