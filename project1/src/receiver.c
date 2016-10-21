//
//  main.c
//  LINGI1341-Project1
//
//  Created by Jsday on 14/10/16.
//  Copyright © 2016 Reseaux informatique. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getopt
#include <getopt.h>
#include <zlib.h> //crc32
#include <string.h>
#include "packet_interface.h"
#include "real_address.h"
#include "read_write_loop.h"
#include "create_socket.h"
#include "wait_for_client.h"

void parse_parameters(int argc, char **argv);

char opt;
char *file = NULL;
char *hostname = NULL;
int port = -1;
#define BUF_SIZE 500

int main(int argc, char ** argv){
    
    //Initialization
    parse_parameters(argc, argv);
    
    /* Resolve the hostname */
    struct sockaddr_in6 addr;
    const char *err = real_address(hostname, &addr);
    if (err) {
        fprintf(stderr, "Could not resolve hostname %s: %s\n", hostname, err);
        return EXIT_FAILURE;
    }
    /* Get a socket */
    int sfd = create_socket(&addr, port, NULL, -1);
    if(sfd < 0)
    {
        fprintf(stderr, "Failed to create the socket!\n");
        return EXIT_FAILURE;;
    }
    
    fprintf(stderr,"Wainting for a client..\n");
    int wait = wait_for_client(sfd);
    if(wait < 0)
    {
        fprintf(stderr,
                "Could not connect the socket after the first message.\n");
        close(sfd);
        return EXIT_FAILURE;;
    }
    fprintf(stderr,"Got a client..\n");
    
     /*Process I/O
    //read_write_loop(sfd);*/
    
    close(sfd);
    
    return EXIT_SUCCESS;
    
}

void parse_parameters(int argc, char **argv){
    if (argc < 2) {
        fprintf(stderr, "`%s' arguments missing\n", argv[0]);
        fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
        exit(1);
    }
    else {
        /* specify which parameters we expect */
        static struct option options[] =
        {
            {"filename", required_argument, 0, 'f'},
            {"help", no_argument, NULL, 'h'},
            {NULL, 0, NULL, 0}
        };
        
        while ((opt = getopt_long(argc, argv, "f:h:", options, NULL)) != -1) {
            switch (opt) {
                case 'f':
                    file = optarg;
                    break;
                case 'h':
                    printf("\n");
                    printf("Usage: %s hostname port\n", argv[0]);
                    printf("Usage: %s [-f X] hostname port\n", argv[0]);
                    printf("Usage: %s [--filename X] hostname port\n", argv[0]);
                    printf("\n");
                    exit(1);
                default:
                    fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
                    exit(1);
            }
        }
        
        hostname = argv[argc-2];
        port = atoi(argv[argc-1]);
    }
    
}
