/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: earteshc <earteshc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 19:26:15 by earteshc          #+#    #+#             */
/*   Updated: 2017/11/16 14:33:37 by earteshc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	**ft_free_str_arr(char ***str_arr)
{
	size_t i;

	i = 0;
	if (str_arr == NULL)
		return (NULL);
	while ((*str_arr)[i] != 0)
		i++;
	while (i != 0)
	{
		free((*str_arr)[i - 1]);
		i--;
	}
	free(*str_arr);
	return (NULL);
}
