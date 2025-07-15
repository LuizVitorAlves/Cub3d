/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:57:55 by lalves-d          #+#    #+#             */
/*   Updated: 2025/07/15 15:59:42 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//Verifica se a string começa com o prefixo dado.
int starts_with(const char *str, const char *prefix)
{
    int i = 0;
    while (prefix[i])
    {
        if (str[i] != prefix[i])
            return (0);
        i++;
    }
    return (1);
}

//Duplica uma string, alocando memória para ela.
char *ft_strdup(const char *s)
{
    char    *new;
    int     len = strlen(s);
    int     i = 0;

    new = malloc(len + 1);
    if (!new)
        return (NULL);
    while (s[i])
    {
        new[i] = s[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

//Copia o mapa temporário para o mapa final, alocando memória.
char **copy_map(char **temp_map, int size)
{
    char **map;
    int i;

    map = malloc(sizeof(char *) * (size + 1));
    if (!map)
        return (NULL);
    i = 0;
    while (i < size)
    {
        map[i] = ft_strdup(temp_map[i]);
        i++;
    }
    map[i] = NULL;
    return (map);
}
