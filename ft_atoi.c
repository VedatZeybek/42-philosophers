#include "philosophers.h"

static int	ft_skipspace(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	size_t		i;
	int			sign;
	long		result;
	int			control;

	result = 0;
	if (str[0] == '\0')
		return (result);
	i = ft_skipspace(str);
	sign = 1;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	result = result * sign;
	return (result);
}
