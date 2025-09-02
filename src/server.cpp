#include <iostream>      //used for cout and cin 
#include <sys/socket.h>  //for socket system call
#include <netinet/in.h>  // for address strucute
#include <string.h>      //for strlen
#include <arpa/inet.h>   //for inet_pton converting it string into ip address
#include <unistd.h>      //for close()
#include <sys/select.h>  //for select used in non blocking tcp for multiple client
#include <vector>        //for storing client 
#include <cerrno>        //for error
#include <fcntl.h>
#include <algorithm>


constexpr int PORT = 12960;         //compile-time constant these are fixed at compile time and do not change at runtime
constexpr int MAX_BUFFER = 1024;    //


void make_nonblocking(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1)
    {
        perror("fcntl get");
        exit(1);
    }
    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl set");
        exit(1);
    }
}


std::vector<int> clients;       //for storing file descriptor for communication socket


void remove_client(int s){
    clients.erase(std::remove(clients.begin(), clients.end(), s), clients.end());
    close(s);
}


int main(){
    

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);                                           
    if(server_socket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }


    int opt = 1;
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("opt failed");
        return 1;
    }


    sockaddr_in server_address{}; //datatype for holding address of server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);  
    server_address.sin_addr.s_addr = INADDR_ANY;

    
    if(bind(server_socket, (sockaddr *)&server_address, sizeof(server_address)) < 0) //bind can work with sockaddr_in and sockaddr_in6 to this bind need a pointer to sockaddr
    {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

  
    if(listen(server_socket, SOMAXCONN) < 0)
    {
        perror("listening failed");
        close(server_socket);
        return 1;
    }


    make_nonblocking(server_socket);


    while(true){

        sockaddr_in client_address{};
        socklen_t addrlen = sizeof(client_address);
        int communication_socket = accept(server_socket, (sockaddr *)&client_address, &addrlen); 

        if(communication_socket >= 0)
        {
            make_nonblocking(communication_socket);
            clients.push_back(communication_socket);
            std::cout << "New client connected \n" ;
        }

        else if(communication_socket < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept failed");
        }

        for(auto it = clients.begin(); it != clients.end();){
            int socket =  *it;
            char buffer[MAX_BUFFER];
            int nbytes = recv(socket, buffer, sizeof(buffer), 0);

            if(nbytes > 0)
            {
                send(socket, buffer, nbytes, 0);
                ++it;
            }
            else if(nbytes == 0)
            {
                std::cout <<"Client " << socket << "disconnected\n";
                remove_client(socket);
                it = clients.erase(it);
            }
            else
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    ++it;
                }
                else
                {
                    perror("recv");
                    remove_client(socket);
                    it = clients.erase(it);
                }
            }
        }
    }


    close(server_socket);
    return 0;


}
