#include <iostream>        // for cout, cin
#include <string>          // for std::string
#include <unistd.h>        // for close()
#include <sys/types.h>     // for data types like socklen_t
#include <sys/socket.h>    // for socket(), bind(), listen(), accept(), connect(), send(), recv()
#include <arpa/inet.h>     // for htons(), htonl(), inet_addr()
#include <netinet/in.h>    // for sockaddr_in, INADDR_ANY
using namespace std;


{// used to create a socket

    sockfd = socket(family,type,protocol)

    family -> AF_INET (IPv4), AF_INET6 (IPv6), AF_UNIX (local communication)
    type -> SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
    portocol -> 0 (default protocol for the given family and type) {IPROTO_TCP for TCP, IPPROTO_UDP for UDP}

    returns a socket descriptor (integer value) on success, -1 on error
}

{// used to close a socket ie frees up the port used by socket
    status = close(sockfd)  
    status -> 0 on success, -1 on error
}

{// address structure for IPv4
    
    {//Generic data Type for address
        struct sockaddr {
            unsigned short sa_family;    // address family, AF_xxx
            char           sa_data[14];  // 14 bytes of protocol address
        };
    }    
    
    {//TCP/IP specific sockaddr structure
        struct sockaddr_in {
            short            sin_family;   // e.g. AF_INET, AF_INET6
            unsigned short   sin_port;     // e.g. htons(3490)
            struct in_addr   sin_addr;     // see struct in_addr, below
            char             sin_zero[8];  // zero this if you want to
        };

        struct in_addr {
            unsigned long s_addr;          // load with inet_pton()
        };
    } 
    
    {//UDP specific sockaddr structure
        struct sockaddr_in6 {
            u_int16_t       sin6_family;   // address family, AF_INET6
            u_int16_t       sin6_port;     // port number, Network Byte Order
            u_int32_t       sin6_flowinfo; // IPv6 flow information
            struct in6_addr sin6_addr;     // IPv6 address
            u_int32_t       sin6_scope_id; // Scope ID
        };

        struct in6_addr {
            unsigned char   s6_addr[16];   // IPv6 address
        };
    }
    
    {//Way to assign the address
        struct sockaddr_in addrport;
        
        addrport.sin_family = AF_INET; // IPv4
        addrport.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
        addrport.sin_port = htons(8080); // Port number (convert to network byte order)
    }
    
}

{//Binding address to port

    // can be skipped if the port is used only for sending data on UDP client side
    // For TCP client side, bind is optional as connect does it implicitly 

    status = bind(sockfd, (struct sockaddr *)&addrport, sizeof(addrport))
    
    status -> 0 on success, -1 on error
}    

{// listening (used for TCP server side only) as UDP is connectionless
// marks the socket as a passive socket that will be used to accept incoming connection requests

    status = listen(sockfd, backlog)
    
    backlog -> maximum number of pending connections that can be queued up before connections are refused
    
    status -> 0 on success, -1 on error
}

{// connect (used for TCP client side only) as UDP is connectionless)

    // blocking in nature, the function call will block until the connection is established or fails

    status = connect(sockfd, (struct sockaddr *)&addrport, sizeof(addrport))

    sockfd -> socket descriptor for client sigde
    addrport -> address and port of the server to connect to
    size -> size of the address structure
    
    status -> 0 on success, -1 on error

}   

{// accept (used for TCP server side only) as UDP is connectionless
    
    // used to accept incoming connection requests from clients
    int new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len) 
    
    new_socket -> new socket descriptor for the accepted connection
    client_addr -> structure to hold the address of the connecting client
    addr_len -> size of the client address structure

    new_socket -> new socket descriptor for the accepted connection on success, -1 on error

    // accept is a blocking call, it will block until a connection request is received
}

{// exchanging data

    {// sending and recieving for TCP (stream socket) (connection-oriented)
        int count = send(sockfd, buffer, len, flags)
        
        sockfd -> socket descriptor of the connected socket
        buffer -> pointer to the data to be sent
        len -> length of the data to be sent
        flags -> 0 (default), can be used to specify special options like MSG_DONTWAIT, MSG_OOB etc.

        count -> number of bytes sent, -1 on error




        int count = recv(sockfd, buffer, len, flags)    

        sockfd -> socket descriptor of the connected socket
        buffer -> pointer to the buffer to store the received data
        len -> length of the buffer
        flags -> 0 (default), can be used to specify special options like MSG_DONTWAIT, MSG_OOB etc.

        count -> number of bytes received, 0 if the connection is closed, -1 on error
    }   

    {// sending and recieving for UDP (datagram socket) (connectionless-oriented)

        int count = sendto(sockfd, buffer, len, flags, (struct sockaddr *)&dest_addr, addrlen)
        
        sockfd -> socket descriptor
        buffer -> pointer to the data to be sent
        len -> length of the data to be sent
        flags -> 0 (default), can be used to specify special options like MSG_DONTWAIT, MSG_OOB etc.
        dest_addr -> address of the destination
        addrlen -> size of the address structure

        count -> number of bytes sent, -1 on error


        int count = recvfrom(sockfd, buffer, len, flags, (struct sockaddr *)&src_addr, &addrlen)    

        sockfd -> socket descriptor
        buffer -> pointer to the buffer to store the received data
        len -> length of the buffer 
        flags -> 0 (default), can be used to specify special options like MSG_DONTWAIT, MSG_OOB etc.
        src_addr -> structure to store the address of the sender
        addrlen -> size of the address structure
        
        count -> number of bytes received, -1 on error
     
    }   



    

}






