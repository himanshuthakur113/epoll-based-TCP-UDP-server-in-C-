
#include <iostream>
#include <sys/socket.h> //for socket system call
#include <netinet/in.h> // for address strucute
#include <string.h>     //for strlen
#include <arpa/inet.h>  //for inet_pton converting it string into ip address
#include <unistd.h>     //for close()

const int port = 12960;

int main(){
   
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
                                                
    if(server_socket == -1)
    {
        std::cerr << "socket creation failed" <<std::endl;
        return -1;
    }

    sockaddr_in server_address; //datatype for holding address of server

    int address_length = sizeof(server_address);

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

  
    if(listen(server_socket,5) < 0)
    {
        std::cerr << "listen failure" <<std::endl;
    }
                                  //so now the socket is waiting for client to connect
                            //5 is backlog the max no. of pending request the os should hold for this socket these request has completed a 3 way handshake
    std::cout << "Listening for connection" << port <<std::endl;
 
    
    int communication_socket = accept(server_socket,(struct sockaddr*)&server_address,(socklen_t*)&address_length);  //created a new socket which take its data from old socket since it is server this process will handle multiple clients so one socket form connecting to client and other for communication 
                                                                                                                          //we used the accept call here and the third parameter we passed is pointer to struct socklen_t which is size of server address before intialization telling the accept what would be size and after accept modify it to actual size

    if(communication_socket < 0)
    {
        std::cerr << "communication socket failed" <<std::endl;
    }

    std::cout << "connection accepted from client" <<std::endl;

    char buffer[1024] = {0};

    recv(communication_socket,buffer,1024,0);

    std::cout << "client buffer" << buffer << std::endl;

    close(communication_socket);
    close(server_socket);
    
    return 0;
}
