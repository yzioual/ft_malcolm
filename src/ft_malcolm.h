#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../libft/libft.h"
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>  /* struct ether_arp, ARPHRD_ETHER, ARPOP_REQUEST */
#include <ifaddrs.h> /* get_interface structs */
#include <net/if.h>        /* if_nametoindex */
#include <netpacket/packet.h> /* sockaddr_ll */
#include <stddef.h>
#include <unistd.h>


typedef struct s_options
{
    char            *src_ip_str;
    char            *src_mac_str;
    char            *target_ip_str;
    char            *target_mac_str;
    bool verbose_mode;
    struct in_addr  src_ip;
    struct in_addr  target_ip;
    unsigned char   src_mac[6];
    unsigned char   target_mac[6];
} t_options;

typedef struct s_session {
    int sockfd;
    struct sockaddr_ll sll;
} t_session;

void handle_sigint(int sig);
int create_socket(t_session *session, char *interface);
int parse_mac(const char *mac_str, unsigned char *mac_out);
char *get_interface();
int send_arp_reply(t_session *session, t_options *opts);
int fill_opts(char **av, t_options *opts);
void free_options(t_options *opts);
int work(t_options *opts);

#endif
