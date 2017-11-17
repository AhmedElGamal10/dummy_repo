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


using namespace std;

vector<string> parse_file(string file_name);
vector<string> split (string s, string delimiter);
string get_request(string request);
string getFileType(vector<string> request);
vector<string> parse_request(string request);


#endif


// questions?
/*
the use of port number and host name :V
*/