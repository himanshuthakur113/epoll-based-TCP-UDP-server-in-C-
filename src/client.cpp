#include <iostream>
#include <sys/socket.h> //for socket system call
#include <netinet/in.h> // for address strucute
#include <string.h>     //for strlen
#include <arpa/inet.h>  //for inet_pton converting it string into ip address
#include <unistd.h>     //for close()

const int server_port = 12960;
const char* server_ip = "127.0.0.1"; //pc has multiple ip address for connecting with iteself it used a loopback address 

int main(){
    
    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    if(client_socket < 0)
    {
        std::cerr << "socket creation failed" << std::endl;
    }

    //for sending date there is no address binding, directly intiates connect() with server known ip and port.os handle the binding of this socket
    //for connect we need server ip and port 

    
    sockaddr_in server_address;
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    if(inet_pton(AF_INET,server_ip,&server_address.sin_addr) <= 0) //convet server address string into network format
    {
        std::cerr << "invalid server address" <<std::endl;
        return -1;
    }

    if(connect(client_socket, (struct sockaddr*)server_address,sizeof(server_address)) < 0)
    {
        std::cerr<<"connection failed" << std::endl;
        return -1;
    }

    //connect intiates a tcp handshake to establish a connection SYN,SYN_ACK,ACK

    std::cout << "Connected to the server" <<std::endl;

    char message[] = {"Hello from client"};


    send(client_socket,message,strlen(message),0);
    
    //this send the message to kernal buffer associated with this socket 
    //after buffere passes to the date to kernal tcp/ip stack 
    //where in transport layer the host and destination port number are assigned to data packets with sequence no.(only in tcp)
    //they are converted it either into segment or datagram
    //in the next internet layer ip address header are assigned 
    //in the link layer combination of physical or data link the mac address is assigned 
    //and the kernal pass the data to the nic buffer here nic driver come into use 
    //this passing from kernal to nic is done by dma without cpu intervention
    
    close();

    return 0;
}
    


