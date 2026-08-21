#include "ft_malcolm.h"

int main(int ac, char **av) {
    struct sigaction sa_int;

    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_int, NULL);

    if (ac != 5) {
        ft_putstr_fd("Usage: ./ft_malcolm <source ip> <source mac> <target ip> <target mac>\n", 2);
        return 1;
    }

    t_options opts;
    ft_memset(&opts, 0, sizeof(opts));

    if (fill_opts(av, &opts) < 0) {
        free_options(&opts);
        return 1;
    }

    if (getuid() != 0) {
        ft_putstr_fd("ft_malcolm: root privileges required.\n", 2);
        free_options(&opts);
        return 1;
    }

    int ret = work(&opts);
    free_options(&opts);
    if (ret < 0)
        return 1;
    return 0;
}
