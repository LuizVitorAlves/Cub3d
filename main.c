/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 15:00:28 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 16:12:15 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

int main(int argc, char **argv)
{
    t_config cfg;
    int i;

    if (argc != 2)
    {
        printf("Uso: ./cub3d mapa.cub\n");
        return (1);
    }
    memset(&cfg, 0, sizeof(t_config));
    if (parse_cub_file(argv[1], &cfg))
    {
        free_config_and_map(&cfg);
        return (1);
    }

    printf("NO: %s\n", cfg.no_path);
    printf("SO: %s\n", cfg.so_path);
    printf("WE: %s\n", cfg.we_path);
    printf("EA: %s\n", cfg.ea_path);
    printf("F: %d\n", cfg.floor_color);
    printf("C: %d\n", cfg.ceiling_color);
    printf("\n--- MAPA ---\n");
    i = 0;
    while (cfg.map && cfg.map[i])
        printf("%s", cfg.map[i++]);
    printf("\n");
    
    free_config_and_map(&cfg);
    
    return (0);
}
