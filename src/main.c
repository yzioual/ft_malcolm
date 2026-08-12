#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "../libft/libft.h"

volatile sig_atomic_t g_running = 1;

typedef struct s_options
{
    bool verbose_mode;
    int target_count;
    char **targets;
} t_options;

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

// TODO: main while that will listen for an ARP request and sends back an ARP reply.
void work()
{
    while (g_running)
        sleep(30);
}

int main(int ac, char **av)
{
    struct sigaction sa_int;
    sa_int.sa_flags = 0;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_int, NULL);

    int ret = parse_options(ac, av);
    if (ret < 0 || ret == 1)
        return 1;

    work();

    return 0;
}
