#include "libft.h"

int	ft_words_count(char *s)
{
	int	i;
	int	in_word;
	int	count;

	count = 0;
	in_word = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] <= 32)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}
