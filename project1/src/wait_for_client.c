//
//  wait_for_client.c
//  Chat
//
//  Created by Jsday on 28/09/16.
//  Copyright © 2016 Reseaux informatique. All rights reserved.
//

#include "wait_for_client.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 520
int wait_for_client(int sfd)
{
    
    char buffer[520];
    struct sockaddr_storage address;
    socklen_t address_len;
    ssize_t nread;
    int serv;
    /*struct sockaddr_in6 *address = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6));
    socklen_t address_len = sizeof(*address);
    int rcv,con;
    
    printf(">>>>>>>Test 0\n");
    rcv = (int)recvfrom(sfd,buffer,520,MSG_PEEK,(struct sockaddr *)address,&address_len);
    printf(">>>>>>>Test 1\n");
    con = connect(sfd,(struct sockaddr *)address,address_len);
    
    if(rcv == -1 || con == -1)
        return -1;
    
    return 0;*/
    
    for (;;) {
        address_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buffer, BUF_SIZE, 0,
                         (struct sockaddr *) &address, &address_len);
        if (nread == -1)
            return -1;
        
        char host[NI_MAXHOST], service[NI_MAXSERV];
        
        serv = getnameinfo((struct sockaddr *) &address,
                        address_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);
        if (serv == 0){
            printf("Received %ld bytes from %s:%s\n",
                   (long) nread, host, service);
            printf("%s\n", buffer);
        }
        else{
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(serv));
        }
        
        if (sendto(sfd, buffer, nread, 0,
                   (struct sockaddr *) &address,
                   address_len) != nread)
            fprintf(stderr, "Error sending response\n");
    }
    
    /*con = connect(sfd, address,address_len);
    
    if(con < 0)
        return -1;*/

    
    return 0;
}

