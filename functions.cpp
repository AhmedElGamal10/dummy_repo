#include "functions.h"

// Parsing the command files.
vector<string> parse_file(string file_name) {
    vector<string> result;
    ifstream inFile;
    inFile.open(file_name.c_str());

    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        // exit(1);   // call system to stop
    }
    string x;
    while (std::getline(inFile, x)) {  
        // getline doesn't read new line :3, so no need to check for newlines.
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

// Returns one HTTP request.
string get_request(string request) {
    string result;
    string file_type;
    vector<string> client_request = split(request,SPACE);
    for (int i = 0; i < client_request.size() - 1; i++) { // size-1 => port number not used
        if (i == 2) { 
            result.append(HTTP);
            result += NEWLINE;
            result += HOST;
            result += SPACE; 
        }
        result.append(client_request.at(i));
        result += SPACE;
    }
    // add carriage return and new line at end of request.
    result += CARRiAGERETURN;  
    result += NEWLINE;
    return result;
}

string getFileType(vector<string> request){
    string fileName = request[1];
    vector<string> fileNameTokens = split(fileName, ".");
    
    for(int i = 0; i < fileNameTokens.size(); i++){
        cout << "string in vector is:" << fileNameTokens[i] << endl;
    }

    string fileExtension = fileNameTokens[1];
    string imgFileType = "img";
    
    if(fileExtension.compare("txt") != 0 && fileExtension.compare("html") != 0)
        return imgFileType;
    else
        return fileExtension;
}

vector<string> parse_request(string request) {
    vector<string> result;
    //  Assuming the request ends with \r\n, split at them.
    string delimiter = CARRiAGERETURN;
    delimiter += NEWLINE;
    // This will happen if the server received more than one request at the buffer. can this happen?
    // assuming not we will process only requests.at(0)
    vector<string> requests = split(request, delimiter);
    // process a request.

    /*split at newline, will give two parts as :
    * GET ttt.txt HTTP/1.1
    * Host: www.tutorialspoint.com 
    */
    vector<string> requestLines = split(requests.at(0), NEWLINE); //2 lines.
    for (int j = 0; j < requestLines.size(); j++) {
        vector<string> s = split(requestLines.at(j), SPACE);
        result.insert(result.end(), s.begin(), s.end());
    }

    // GET => ttt.txt => HTTP/1.1 => Host: => www.tutorialspoint.com
     return result;
}

void recvImg(int new_fd){
    
}

void recvTxt(int new_fd){

}

void recvHTML(int new_fd){

}

int sendTxt(int new_fd, std::vector<std::string> curRequest){

        char file_size[256];
        struct stat file_stat;
        int offset;
        int remain_data;
        ssize_t len;
        int fd;
        int sent_bytes = 0;

        string fileName = curRequest[1];

        char *FILE_TO_SEND = new char[fileName.length() + 1]; // or
        // char y[100];
        strcpy(FILE_TO_SEND, fileName.c_str());

        cout << "aa" << endl;
        printf("hehehehe%s\n", FILE_TO_SEND);
        fd = open(FILE_TO_SEND, O_RDONLY);
        cout << "bb" << endl;

        if (fd == -1){
            fprintf(stderr, "Error opening file --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Get file stats */
        if (fstat(fd, &file_stat) < 0){
            fprintf(stderr, "Error fstat --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        sprintf(file_size, "%d", file_stat.st_size);

        /* Sending file size */
        len = send(new_fd, file_size, sizeof(file_size), 0);
        if (len < 0){
            fprintf(stderr, "Error on sending greetings --> %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        offset = 0;
        remain_data = file_stat.st_size;
        off_t ot = (off_t) offset;
              /* Sending file data */
        while (((sent_bytes = sendfile(new_fd, fd, &ot, BUFSIZ)) > 0) && (remain_data > 0)){
                fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        }

        close(new_fd);
}

int sendHTML(int new_fd, std::vector<std::string> curRequest){

}

int sendImg(int new_fd, std::vector<std::string> curRequest){
    
}