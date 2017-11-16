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

char** split (char* input, char** args)
{

  char* pch = strtok (input," ");
  int i = 0;

  while (pch != NULL)
  {
    printf ("%s\n",pch);
    args[i++] = strtok (NULL, " ");
  }

  return args;
}

void recvTxt(int new_fd){

}

void recvImg(int new_fd){

}

void recvHTML(int new_fd){

}

void sendTxt(int new_fd, std::vector<std::string> curRequest){

}


void sendHTML(int new_fd, std::vector<std::string> curRequest){

}


void sendImg(int new_fd, std::vector<std::string> curRequest){

}

void handlePostRequest(int new_fd, std::vector<std::string> curRequest){
    if (send(new_fd, OK_MSG, 2, 0) == -1)
    	perror("POST request not OK reply");

    std::string fileType = curRequest[3];

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
	
	std::string fileType = curRequest[3];
	int result;

    if(fileType.compare("txt") == 0){
    	sendTxt(new_fd, curRequest);
    }else if(fileType.compare("html") == 0){
		sendHTML(new_fd, curRequest);
    }else if(fileType.compare("img") == 0){
    	sendImg(new_fd, curRequest);
    }else{
		printf("(GET request) file type not recognised\n");
    }

	if(result == -1){
		printf("file not found");
	}
}


std::string prepareReponse(std::vector<std::string> curRequest){
	std::string reponse = OK_MSG;
	
	int rqst_type;
	if(curRequest[0].compare("GET") == 0)
		rqst_type = GET_TYPE;
	else
		rqst_type = POST_TYPE;

	std::string fileName = curRequest[1];
	if(rqst_type == GET_TYPE){
		if(checkFileExisting(fileName)){
			return response;
		}else{
			return error_msg;
		}
	}else{
		return reponse;
	}

}


void sendResponse(int new_fd, std::vector<std::string> curRequest){

	std::string response = prepareReponse(curRequest);

    if (send(new_fd, response, 2, 0) == -1) 
    	perror("send reponse to client: ");
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
            // if (send(new_fd, "Hello, world!", 13, 0) == -1) perror("send");
            char buf[MAXDATASIZE];
            int numbytes;
            
            // if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1)
            // {
            //     perror("recv");
            //     exit(1);
            // }

            printf("hamada\n");
			std::vector<std::vector<std::string> > buffer(VEC_MAX_SIZE);
			// int result = recv(new_fd, buffer.data(), buffer.size(), 0);
			// if (result != -1) {
			//    buffer.resize(result);
			// } else {
			//    // Handle error
			// }
			printf("hamada2\n");
            std::vector<std::vector<std::string> > v (4);	
		    for(int i = 0; i < 4; i++){
		        std::vector<std::string> temp;
		        temp.push_back("GET");
		        temp.push_back("ahmed.jpg");
		        temp.push_back("HTTP/1.1");
		        temp.push_back("img");
		        v.push_back(temp);
		    }

			printf("hamada3\n");
			// for(int i = 0; i < VEC_MAX_SIZE; i++){
			buffer = v;
			for(int i = 0; i < 4; i++){

				std::vector<std::string> curRequest = buffer[i];


	            int rqst_type;
	            if(curRequest[0].compare("GET") == 0)
	                rqst_type = GET_TYPE;
	            else
	                rqst_type = POST_TYPE;

	            printf("rqst type is: %d\n", rqst_type);
	            if(rqst_type == POST_TYPE){
    	            handlePostRequest(new_fd, curRequest);
    	            // if (send(new_fd, "Hello, world!", 13, 0) == -1) perror("send");
	            }else{
	            	handleGetRequest(new_fd, curRequest);
	            }

	            sendResponse();
			}

            printf("%s\n", buf);


            close(new_fd);
            exit(0);
        }
        // close(new_fd);  // parent doesn't need this
    }
    return 0;
}
