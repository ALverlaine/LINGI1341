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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void parse_parameters(int argc, char **argv);
int file_read(int sfd, int fd);

/* Global variable */
char opt;
char *file = NULL;
char *hostname = NULL;
int port = -1;
#define BUF_SIZE 500


int main(int argc, char **argv){
    
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
    
    int sfd = create_socket(NULL, -1, &addr, port); /* Connected */
    if(sfd < 0)
    {
        fprintf(stderr, "Failed to create the socket!\n");
        return EXIT_FAILURE;
    }
    /* First message */
    char *hello = "hello";
    write(sfd,hello,sizeof(hello));
    
    /* No file specified */
    if (file == NULL) {
        printf("Write a message to send:\n");
        /* Process I/O */
        read_write_loop(sfd);
    }
    else{
        /* Open in reading mode*/
        int f = open(file, O_RDONLY);
        
        if(file_read(sfd, f) < 0){
            close(f);
            close(sfd);
            return EXIT_FAILURE;
        }
        
        close(f);
    }
    
    close(sfd);

    return EXIT_SUCCESS;
    
}

int file_read(int sfd, int fd){
    char buffer[BUF_SIZE];
    ssize_t r = read(fd, buffer, sizeof(buffer));
    
    int w = (int) write(sfd,buffer,r);
    if(w == -1){
        fprintf(stderr, "error with write (sender)");
        return -1;
    }

    return 0;
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


