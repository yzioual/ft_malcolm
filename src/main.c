#include "ft_malcolm.h"

int main(int ac, char **av) {
  struct sigaction sa_int;
  sa_int.sa_flags = 0;
  sigemptyset(&sa_int.sa_mask);
  sa_int.sa_handler = handle_sigint;
  sigaction(SIGINT, &sa_int, NULL);

  if (ac != 5) {
    ft_putstr_fd("[ft_malcolm] not enough arguments.\n", 2);
    return -1;
  }

  t_options opts;
  ft_memset(&opts, 0, sizeof(opts));
  int ret = fill_opts(av, &opts);
  if (ret < 0) {
    ft_putstr_fd("[ft_malcolm] Error occurred while parsing cmd args.\n", 2);
    return -1;
  }

  if (getuid() != 0) {
    ft_putstr_fd(
        "[ft_malcolm]: root privileges required to create raw sockets.\n", 2);
    return -1;
  }
  ret = work(opts);
  if (ret < 0)
    return -1;
  return 0;
}
