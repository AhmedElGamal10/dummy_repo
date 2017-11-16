/*
** server.c -- a stream socket server demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define GET_TYPE 1
#define POST_TYPE 0
#define VEC_MAX_SIZE 5000
#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define OK_MSG "HTTP/1.0 200 OK\r\n"
#define ERR_MSG "HTTP/1.0 404 Not Found\r\n"

void sigchld_handler(int s)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool checkFileExisting(std::string fileName){

}

void recvTxt(int new_fd){

}

void recvHTML(int new_fd){

}

void recvImg(int new_fd){
	
}

int sendTxt(int new_fd, std::vector<std::string> curRequest){

}

int sendHTML(int new_fd, std::vector<std::string> curRequest){

}

int sendImg(int new_fd, std::vector<std::string> curRequest){
	
}

void sendResponse(int new_fd, char* response){
    if (send(new_fd, response, strlen(response), 0) == -1)
    	perror("send response");
}

void handlePostRequest(int new_fd, std::vector<std::string> curRequest){
	sendResponse(new_fd, OK_MSG);

    std::string fileType = curRequest[4];
    if(fileType.compare("txt") == 0){
    	recvTxt(new_fd);
    }else if(fileType.compare("html") == 0){
		recvHTML(new_fd);
    }else if(fileType.compare("img") == 0){
    	recvImg(new_fd);
    }else{
		printf("(POST request) file type not recognised\n");
    }
}

void handleGetRequest(int new_fd, std::vector<std::string> curRequest){
	
	std::string fileType = curRequest[4];
	int result;
	char* response;
	std::string fileName = curRequest[1];
	if(!checkFileExisting(fileName)){
		 response = ERR_MSG;	
	}else{
		response = OK_MSG;
	}
	
	sendResponse(new_fd, response);
    
    if(fileType.compare("txt") == 0){
    	result = sendTxt(new_fd, curRequest);
    }else if(fileType.compare("html") == 0){
		result = sendHTML(new_fd, curRequest);
    }else if(fileType.compare("img") == 0){
    	result = sendImg(new_fd, curRequest);
    }else{
		printf("(GET request) file type not recognised\n");
    }

	if(result == -1){
		printf("file not found");
	}
}



int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;  // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // use my IP
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }
    freeaddrinfo(servinfo);  // all done with this structure
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler;  // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    printf("server: waiting for connections...\n");
    while (1)    // main accept() loop
    {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        if (!fork())      // this is the child process
        {
            
            close(sockfd);  // child doesn't need the listener
            char buf[MAXDATASIZE];
            int numbytes;
            
			std::vector<std::vector<std::string> > buffer(VEC_MAX_SIZE);
			// int result = recv(new_fd, buffer.data(), buffer.size(), 0);
			// if (result != -1) {
			//    buffer.resize(result);
			// } else {
			//    // Handle error
			// }

			// std::vector<std::string> curRequest = buffer[i];
	        


	        std::vector<std::string> temp;
	        temp.push_back("POST");
	        temp.push_back("ahmed.jpg");
	        temp.push_back("HTTP/1.1");
	        temp.push_back("HostName");
	        temp.push_back("img");			
			std::vector<std::string> curRequest = temp;
            
            if(curRequest[0].compare("GET") == 0)
                handleGetRequest(new_fd, curRequest);
            else
	            handlePostRequest(new_fd, curRequest);

            cout << "finished" << endl;
            close(new_fd);
            exit(0);
        }
        // close(new_fd);  // parent doesn't need this
    }
    return 0;
}