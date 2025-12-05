#include "ft_standard.h"
#include <stdio.h>

static int	ft_get_size(long long n);
static char	*ft_lltoa_pos(long long n);
static char	*ft_lltoa_neg(long long n);

char	*ft_lltoa(long long n)
{
	if (n < 0)
		return (ft_lltoa_neg(n));
	else
		return (ft_lltoa_pos(n));
}

static char	*ft_lltoa_pos(long long n)
{
	int			size;
	long long	n_c;
	int			i;
	char		*res;

	size = ft_get_size(n);
	res = ft_malloc((unsigned long)(size + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	n_c = n;
	while (i < size)
	{
		res[size - 1 - i] = ((char)(n_c % 10)) + '0';
		n_c = n_c / 10;
		i++;
	}
	res[i] = '\0';
	return (res);
}

static char	*ft_lltoa_neg(long long n)
{
	int			size;
	long long	n_c;
	int			i;
	char		*res;

	size = ft_get_size(n) + 1;
	res = ft_malloc((unsigned long)(size + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	res[0] = '-';
	i = 0;
	n_c = n;
	while (i < size - 1)
	{
		res[size - 1 - i] = (((char)(-(n_c % 10))) + '0');
		n_c = n_c / 10;
		i++;
	}
	res[size] = '\0';
	return (res);
}

static int	ft_get_size(long long n)
{
	long	n_c;
	int		i;

	n_c = n;
	i = 0;
	while (!(-9 <= n_c && n_c <= 9))
	{
		n_c = n_c / 10;
		i++;
	}
	i++;
	return (i);
}


int	main(void)
{
	printf("ft_lltoa(\"9184991849\") = %s\n", ft_lltoa(9184991849LL));
}
