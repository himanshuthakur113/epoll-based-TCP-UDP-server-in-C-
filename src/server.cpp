#include <iostream>
#include <sys/socket.h> //for socket system call
#include <netinet/in.h> // for address strucute
#include <string.h>     //for strlen
#include <arpa/inet.h>  //for inet_pton converting it string into ip address
#include <unistd.h>     //for close()
#include <sys/select.h>  //for select used in non blocking tcp for multiple client
#include <vector>


const int PORT = 12960;
const int BACKLOG = 5;
std::vector<int> com_socket; //list for communication socket for each client

int main(){

    
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
                                                
    if(server_socket == -1)
    {
        std::cerr << "socket creation failed" <<std::endl;
        return -1;
    }

    int opt = 1;
    if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt)<0) //this is for address already in use error when we exit a server os hold its address and port in
                                                                                   //in TIME_WAIT stoping other to bind to that address or port
                                                                                   //but SO_REUSSEADDR make it to connect to oher
    {
        std::cerr << "setsocketopt error\n";
        return -1;
    }



    sockaddr_in server_address; //datatype for holding address of server

    int address_length = sizeof(server_address);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);  //port are logical endpoint  that is used to direct network traffic to specific process or sevices
                                            //htons convert the unsigned short integer hostshort from host byte order to network byte order(little endian to big endian)

    server_address.sin_addr.s_addr = INADDR_ANY; // s_addr is part of sin_addr which store the ip address the actual value is stored in s_addr
                                                 //a pc can have multiple ip address INADDR_ANY means this socket can accept connection from any of them 

    

   if(bind(server_socket, (struct sockaddr*)(&server_address),sizeof(server_address)) < 0) //bind can work with sockaddr_in and sockaddr_in6 to this bind need a pointer to sockaddr
   {                                                                                       //which contain both sockaddr_in 4 & 6 we need to cast the address into sockaddr pointer
        std::cerr << "Binding failed" <<std::endl;
        return -1;
   }

  
    if(listen(server_socket,BACKLOG) < 0)
    {
        std::cerr << "listen failure" <<std::endl;
    }
                                  //so now the socket is waiting for client to connect
                            //5 is backlog the max no. of pending request the os should hold for this socket these request has completed a 3 way handshake
    std::cout << "Listening for connection" << PORT <<std::endl;

    fd_set readfds;
    size_t valread;
    int maxfd;
    int sd = 0;
    int acitivity;
 
    while(true)
    {
        std::cout << "server waiting for client" <<std::endl;
        
        FD_ZERO(&readfds);
        FD_SET(server_socket,&readfds);
        maxfd = server_socket;
        
        for(auto sd:com_socket)
        {
            FD_SET(sd,&readfds);
            if(sd > maxfd)
            {
                maxfd = sd;
            }

        }


        if(sd > maxfd)
        {
            maxfd = sd;
        }


        activity = select(maxfd+1, &readfds,NULL,NULL,NULL);
        if(activity < 0)
        {
            std::cerr << "select failed\n";
            continue;
        }


        if(FD_ISSET(server_socke

        int communication_socket = accept(server_socket,(struct sockaddr*)&server_address,(socklen_t*)&address_length);  //created a new socket which take its data from old socket since it is server this process will handle multiple clients so one socket form connecting to client and other for communication 
                                                                                                                          //we used the accept call here and the third parameter we passed is pointer to struct socklen_t which is size of server address before intialization telling the accept what would be size and after accept modify it to actual size

    if(communication_socket < 0)
    {
        std::cerr << "communication socket failed" <<std::endl;
        continue;
    }

    std::cout << "connection accepted from client" <<std::endl;

    char buffer[1024] = {0};

    int bytes_recieved = recv(communication_socket,buffer,1024,0);

    std::cout << "client msg" << buffer << std::endl;

    send(communication_socket,buffer,bytes_recieved,0);


    close(communication_socket);

    }
    close(server_socket);
    
    return 0;
}
