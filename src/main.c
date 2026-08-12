#include <iso646.h>
#include <stddef.h>
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
#include <net/if.h>        /* if_nametoindex */
#include <netpacket/packet.h> /* sockaddr_ll */
#include <netinet/if_ether.h>  /* struct ether_arp, ARPHRD_ETHER, ARPOP_REQUEST */


volatile sig_atomic_t g_running = 1;



typedef struct s_params {
    char            *src_ip_str;
    char            *src_mac_str;
    char            *target_ip_str;
    char            *target_mac_str;
    struct in_addr  src_ip;
    struct in_addr  target_ip;
    unsigned char   src_mac[6];
    unsigned char   target_mac[6];
} t_params;

typedef struct s_options
{
    bool verbose_mode;
    int target_count;
    char **targets;
} t_options;

typedef struct s_session {
    int sockfd;
    struct sockaddr_ll sll;
} t_session;

void handle_sigint(int sig) {
    (void)sig;
    g_running = 0;
    ft_putstr_fd("\n[ft_malcolm]\tRecieved Ctrl+c signal, exiting right now...\n", 2);
    exit(EXIT_SUCCESS);
}

void print_help(int ac, char **av)
{
    (void) av;
    if (ac < 5)
    {
        ft_putstr_fd("[ft_malcolm] not enough arguments.\n", 2);
        return;
    }
    return;
}

int add_target(t_options *opts, const char *target) {
    char **new_targets;

    if (target == NULL || *target == '\0')
        return 0;
    new_targets = realloc(opts->targets, sizeof(*opts->targets) * (opts->target_count + 1));
    if (new_targets == NULL)
        return (fprintf(stderr, "ft_malcolm: allocation failed\n"), -1);
    opts->targets = new_targets;
    opts->targets[opts->target_count] = strdup(target);
    if (opts->targets[opts->target_count] == NULL)
        return (fprintf(stderr, "ft_malcolm: allocation failed\n"), -1);
    opts->target_count++;
    return 0;
}

void free_options(t_options *opts) {
    size_t i;

    i = 0;
    while (i < (size_t)opts->target_count) {
        free(opts->targets[i]);
        i++;
    }
    free(opts->targets);
}

int parse_options(int ac, char **av)
{
    t_options opts;
    memset(&opts, 0, sizeof(opts));
    int i = 0;

    if (ac < 5)
    {
        print_help(ac, av);
        return 1;
    }

    i = 1;
    while (i < ac)
    {
        add_target(&opts, av[i]);
        i++;
    }
    return 0;
}

int create_socket(t_session *session)
{
    // why this has to be unsigned int?
    unsigned int ifindex;

    ifindex = if_nametoindex("eth0");
    if (ifindex == 0)
    {
        perror("if_nametoindex failed");
        return (-1);
    }

    memset(&session->sll, 0, sizeof(session->sll));
    session->sll.sll_family = AF_PACKET;
    session->sll.sll_ifindex = (int)ifindex;
    session->sll.sll_protocol = htons(ETH_P_ARP);

    session->sockfd = socket(
            session->sll.sll_family, SOCK_RAW, session->sll.sll_protocol);
    if (session->sockfd < 0)
    {
        ft_putstr_fd("[ft_malcolm] Socket creation failed.\n", 2);
        ft_putstr_fd("[ft_malcolm] Try running the ft_malcolm with sudo.\n", 2);
        return 1;
    }

    if (bind(session->sockfd, (struct sockaddr *)&(session->sll), sizeof(session->sll)) < 0)
    {
        ft_putstr_fd("[ft_malcolm] bind() failed.", 2);
        close(session->sockfd);
        return 1;
    }

    return (session->sockfd);
}


int parse_mac(const char *mac_str, unsigned char *mac_out)
{
    int bytes[6];
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
               &bytes[0], &bytes[1], &bytes[2],
               &bytes[3], &bytes[4], &bytes[5]) != 6)
    {
        return (-1);
    }

    for (int i = 0; i < 6; i++) {
        if (bytes[i] < 0 || bytes[i] > 255)
            return (-1);
        mac_out[i] = (unsigned char)bytes[i];
    }
    return (0);
}

int parse_args(t_params *params)
{
    if (inet_pton(AF_INET, params->src_ip_str, &params->src_ip) != 1) {
        ft_putstr_fd("ft_malcolm: invalid IP address: (%s) ", 2);
        ft_putstr_fd(params->src_ip_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    if (parse_mac(params->src_mac_str, params->src_mac) < 0) {
        ft_putstr_fd("ft_malcolm: invalid mac address: (%s) ", 2);
        ft_putstr_fd(params->src_mac_str, 2);
        ft_putstr_fd(".\n", 2);
        return (-1);
    }

    if (inet_pton(AF_INET, params->target_ip_str, &params->target_ip) != 1) {
        ft_putstr_fd("ft_malcolm: invalid IP address: (%s) ", 2);
        ft_putstr_fd(params->target_ip_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    if (parse_mac(params->target_mac_str, params->target_mac) < 0) {
        ft_putstr_fd("ft_malcolm: invalid mac address: (%s) ", 2);
        ft_putstr_fd(params->target_mac_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    return (0);
}

int work()
{
    t_session session = { NULL };
    char buffer[1024];

    if (create_socket(&session) != 0)
    {
        return 1;
    }

    printf("socket was created successfully.\n");

    while (g_running)
    {
        printf("we are listening for incoming arp requests.\n");
        struct sockaddr_ll src_addr;
        socklen_t addr_len = sizeof(src_addr);

        int bytes = recvfrom(session.sockfd,
                buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addr_len);

        if (bytes < 0)
        {
            printf("[ft_malcolm] recvfrom() failed.\n");
            close(session.sockfd);
            return 1;
        }

        if ((size_t)bytes < sizeof(struct ethhdr) + sizeof(struct ether_arp))
            continue;

        struct ethhdr *eth = (struct ethhdr *)buffer;
        if (ntohs(eth->h_proto) == ETH_P_ARP)
        {
            struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ethhdr));

            /* Check if Hardware is Ethernet (1) and Opcode is ARP Request (1) */
            if (ntohs(arp->ea_hdr.ar_hrd) == ARPHRD_ETHER &&
                ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
            {
                char sender_ip[16];
                char target_ip[16];

                inet_ntop(AF_INET, arp->arp_spa, sender_ip, sizeof(sender_ip));
                inet_ntop(AF_INET, arp->arp_tpa, target_ip, sizeof(target_ip));

                printf("Captured ARP Request:\n");
                printf("  Sender MAC : %02x:%02x:%02x:%02x:%02x:%02x\n",
                       arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
                       arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
                printf("  Sender IP  : %s\n", sender_ip);
                printf("  Target IP  : %s\n", target_ip);
                printf("----------------------------------------\n");
            }
        }
    }

    return 0;
}

int main(int ac, char **av)
{
    struct sigaction sa_int;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_int, NULL);

    // TODO: pass in t_params and fill it with user input
    int ret = parse_options(ac, av);
    if (ret < 0 || ret == 1)
        return 1;

    // TODO: pass in all the params and needed input
    work();

    return 0;
}
