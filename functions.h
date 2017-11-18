#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <iostream>
#include <fstream>

#define HTTP "HTTP/1.1"
#define IMAGE "image"
#define HTML "html"
#define TXT "txt"
#define HOST "Host:"
#define NEWLINE "\n"
#define SPACE " "
#define GET "GET"
#define POST "POST"
#define CARRiAGERETURN "\r"
#define REQUESTTYPE 0
#define FILENAME 1
#define HOSTNAME 4
#define MAXDATASIZE 8 * 1024 // max number of bytes we can get at once
#define OK_MSG "HTTP/1.0 200 OK\r\n"
#define ERR_MSG "HTTP/1.0 404 Not Found\r\n"
#define END_MSG "end"


using namespace std;

vector<string> parse_file(string file_name);
vector<string> split (string s, string delimiter);
string get_request(string request);
string getFileType(vector<string> request);
vector<string> parse_request(string request);
void recvImg(int new_fd);
void recvTxt(int new_fd, string fileName);
void recvHTML(int new_fd);
int sendTxt(int new_fd, std::vector<std::string> curRequest);
int sendHTML(int new_fd, std::vector<std::string> curRequest);
int sendImg(int new_fd, std::vector<std::string> curRequest);


#endif


// questions?
/*
the use of port number and host name :V
*/