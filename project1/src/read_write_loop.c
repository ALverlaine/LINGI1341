//
//  read_write_loop.c
//  Chat
//
//  Created by Jsday on 28/09/16.
//  Copyright © 2016 Reseaux informatique. All rights reserved.
//

#include "read_write_loop.h"
#include "packet_interface.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_SEGMENT_SIZE 1024 //max size that can be sent

void read_write_loop(int sfd) {
    
    fd_set sfds;
    char* buff[MAX_SEGMENT_SIZE];
	size_t l1 = 1024;
	size_t *len = &l1;
    FD_ZERO(&sfds);
    
    while(true)
    {
        FD_SET(STDIN_FILENO,&sfds);
        FD_SET(sfd,&sfds);
        
        select(sfd+1,&sfds,NULL,NULL,NULL);
        
        if(FD_ISSET(STDIN_FILENO,&sfds))
        {
            ssize_t r=read(STDIN_FILENO,buff,MAX_SEGMENT_SIZE);
            
            if(r == EOF){
                break;
            }
			
			pkt_t *pkt = NULL;
			size_t l = *len; //faut verifier si ca fonctionne, faudrait garder len comme il est pour permettre de vraiment montrer ce qu'il reste dans le buffer
			pkt_status_code st = pkt_encode(pkt, (char*) buff, &l);
			// len = len - &l; // retire la taille qu'on vient d'écrire !!! cette ligne ne fonctionne pas!!!
            if(st == PKT_OK)
			{
				int w = (int) write(sfd,pkt,r);
				if(w == -1){
					fprintf(stderr, "error with write (read_write_loop)");
				}
			}
			
        }else if(FD_ISSET(sfd,&sfds))
        {
            ssize_t r=read(sfd,buff,MAX_SEGMENT_SIZE);
            
            if(r == EOF){
                break;
            }
			pkt_t *pkt1 = NULL;
			pkt_status_code st1 = pkt_decode((const char*) buff, sizeof(buff), pkt1);
            if(st1 == PKT_OK)
			{
				int w = (int) write(STDOUT_FILENO,pkt1,r);
				
				if(w == -1){
					fprintf(stderr, "error with write (read_write_loop)");
				}
				//+++ send pkt_ACK si en séquence???
			}
        }
    }
	// faut réaugmenter length qqpart aussi?? plus on mettrait pas read_write_loop dans sender et receiver pour pouvoir accès aux variables globales de ces 2 fichiers là? serait plus facile pour gérer les buffers avec timeout je pense.
}
