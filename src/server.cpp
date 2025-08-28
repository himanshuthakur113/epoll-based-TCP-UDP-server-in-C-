#include <iostream>
#include <sys/socket.h> //used for socket system call
#include <netinet/in.h> // used for storing address

const int port = 12960;

int main(){
   

    int server_socket = socket(AF_INET,SOCK_STREAM,0); //this creates a socket in kernal & return a file descriptor for process decriptor table 
                                                
    if(server_socket == -1)
    {
        std::cerr << "socket creation failed" <<std::endl;
        return -1;
    }

    sockaddr_in server_address;   //datatype for holding address of server

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);  //port are logical endpoint  that is used to direct network traffic to specific process or sevices
                                            //htons convert the unsigned short integer hostshort from host byte order to network byte order(little endian to big endian)

    server_address.sin_addr.s_addr = INADDR_ANY; // s_addr is part of sin_addr which store the ip address the actual value is stored in s_addr
                                                 //a pc can have multiple ip address INADDR_ANY means this socket can accept connection from any of them 
    
   if(bind(server_socket, (struct sockaddr*)(&server_address),sizeof(server_address)) < 0) //bind can work with sockaddr_in and sockaddr_in6 to this bind need a pointer to sockaddr
   {                                                                                       //which contain both sockaddr_in 4 & 6 we need to cast the address into sockaddr pointer
        std::cerr << "Binding failed" <<std::endl;
        return -1;
   }

  
    listen(server_socket,5) //so now the socket is waiting for client to connect
                            //5 is backlog the max no. of pending request the os should hold for this socket these request has completed a 3 way handshake
    std::cout << "Listening for connection" <<std::endl;




    return 0;
}
