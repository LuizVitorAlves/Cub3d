/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_cub.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 14:59:01 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 15:58:02 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

// Função utilitária para verificar se a string é puramente um número
static int is_numeric(char *str)
{
    if (!*str)
        return (0);
    while (*str)
    {
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}

// Adaptação da função de parsing de cor
static int parse_color(const char *line)
{
    int     r, g, b;
    char    *tmp_line;
    char    *part;
    char    *parts[3];
    int     i;

    tmp_line = ft_strdup(line);
    if (!tmp_line)
        return (-1);
    
    i = 0;
    part = strtok(tmp_line, ",");
    while (part && i < 3)
    {
        parts[i++] = part;
        part = strtok(NULL, ",");
    }
    
    if (i != 3 || part != NULL)
    {
        printf(ERROR_MSG "Formato de cor inválido\n");
        free(tmp_line);
        return (-1);
    }

    r = atoi(parts[0]);
    g = atoi(parts[1]);
    b = atoi(parts[2]);
    
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 ||
        !is_numeric(parts[0]) || !is_numeric(parts[1]) || !is_numeric(parts[2]))
    {
        printf(ERROR_MSG "Cor fora do intervalo [0,255] ou formato inválido.\n");
        free(tmp_line);
        return (-1);
    }
    free(tmp_line);
    return ((r << 16) | (g << 8) | b);
}

// Lógica de `is_map_line` para ser mais robusta
static int is_map_line(const char *line)
{
    int i = 0;
    int has_map_char = 0;

    while (line[i] == ' ')
        i++;
    if (line[i] == '\0' || line[i] == '\n')
        return (0);
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != '1' && line[i] != '0' && line[i] != ' ' &&
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
        {
            return (-1); // Retorna -1 para indicar um caractere inválido
        }
        if (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
            has_map_char = 1;
        i++;
    }
    return (has_map_char);
}

// Função de parsing agora valida o caminho da textura
static int parse_config_line(char *line, t_config *cfg)
{
    char **target_path = NULL;

    if (starts_with(line, "NO "))
        target_path = &cfg->no_path;
    else if (starts_with(line, "SO "))
        target_path = &cfg->so_path;
    else if (starts_with(line, "WE "))
        target_path = &cfg->we_path;
    else if (starts_with(line, "EA "))
        target_path = &cfg->ea_path;
    
    if (target_path)
    {
        if (*target_path != NULL)
        {
            printf(ERROR_MSG "Configuração de textura duplicada\n");
            return (1);
        }
        char *path_start = line + 2;
        while (*path_start == ' ') path_start++;
        *target_path = ft_strdup(path_start);
        int len = strlen(*target_path);
        if (len > 0 && (*target_path)[len - 1] == '\n')
            (*target_path)[len - 1] = '\0';
        
        // Valida se o arquivo existe
        int fd = open(*target_path, O_RDONLY);
        if (fd < 0)
        {
            printf(ERROR_MSG "Caminho de textura inválido: %s\n", *target_path);
            return (1);
        }
        close(fd);
    }
    return (0);
}

// Funções para validação do mapa com flood fill
static void flood_fill(char **map, int x, int y, int max_x, int max_y, int *is_valid)
{
    if (x < 0 || y < 0 || y >= max_y)
    {
        *is_valid = 0;
        return;
    }
    if (x >= (int)strlen(map[y]) || map[y][x] == ' ') {
        *is_valid = 0;
        return;
    }
    
    char c = map[y][x];
    if (c == '1' || c == 'V')
        return;

    map[y][x] = 'V';

    flood_fill(map, x + 1, y, max_x, max_y, is_valid);
    flood_fill(map, x - 1, y, max_x, max_y, is_valid);
    flood_fill(map, x, y + 1, max_x, max_y, is_valid);
    flood_fill(map, x, y - 1, max_x, max_y, is_valid);
}

static int find_and_validate_player(t_config *cfg, int *player_x, int *player_y)
{
    int i = 0;
    int j;
    int player_count = 0;

    while (cfg->map[i])
    {
        j = 0;
        while (cfg->map[i][j])
        {
            if (strchr("NSWE", cfg->map[i][j]))
            {
                player_count++;
                *player_x = j;
                *player_y = i;
            }
            j++;
        }
        i++;
    }
    if (player_count != 1)
        return (1);
    return (0);
}

// Função principal de validação
int validate_map(t_config *cfg)
{
    int player_x, player_y;
    char **map_copy;
    int is_valid = 1;

    if (find_and_validate_player(cfg, &player_x, &player_y))
    {
        printf(ERROR_MSG "Número de posições de jogador inválido (deve ser 1).\n");
        return (1);
    }
    
    map_copy = copy_map(cfg->map, cfg->map_height);
    if (!map_copy)
    {
        printf(ERROR_MSG "Erro de alocação ao validar o mapa.\n");
        return (1);
    }
    
    // Inicie o flood fill a partir da posição do jogador
    flood_fill(map_copy, player_x, player_y, strlen(map_copy[player_y]), cfg->map_height, &is_valid);
    
    int i = 0;
    while(map_copy[i])
        free(map_copy[i++]);
    free(map_copy);
    
    if (!is_valid)
        printf(ERROR_MSG "Mapa não é fechado.\n");

    return (!is_valid);
}


int parse_cub_file(char *filename, t_config *cfg)
{
    int     fd;
    char    *line;
    char    *temp_map[MAX_MAP_LINES];
    int     map_line_count = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (printf(ERROR_MSG "Não foi possível abrir o arquivo do mapa.\n"), 1);
    
    int config_count = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\n')
            trimmed++;
        if (*trimmed == '\0')
        {
            free(line);
            continue;
        }

        // Nova lógica de verificação
        if (starts_with(trimmed, "NO ") || starts_with(trimmed, "SO ") || 
            starts_with(trimmed, "WE ") || starts_with(trimmed, "EA "))
        {
            if (cfg->is_in_map_section)
            {
                printf(ERROR_MSG "Configurações após o início do mapa\n");
                free(line);
                close(fd);
                return (1);
            }
            if (parse_config_line(trimmed, cfg)) { free(line); close(fd); return (1); }
            config_count++;
        }
        else if (starts_with(trimmed, "F "))
        {
            if (cfg->is_in_map_section)
            {
                printf(ERROR_MSG "Configurações após o início do mapa\n");
                free(line);
                close(fd);
                return (1);
            }
            cfg->floor_color = parse_color(trimmed + 1);
            if (cfg->floor_color == -1) { free(line); close(fd); return (1); }
            config_count++;
        }
        else if (starts_with(trimmed, "C "))
        {
            if (cfg->is_in_map_section)
            {
                printf(ERROR_MSG "Configurações após o início do mapa\n");
                free(line);
                close(fd);
                return (1);
            }
            cfg->ceiling_color = parse_color(trimmed + 1);
            if (cfg->ceiling_color == -1) { free(line); close(fd); return (1); }
            config_count++;
        }
        else // Não é uma linha de configuração
        {
            int map_check = is_map_line(trimmed);
            if (map_check == -1)
            {
                printf(ERROR_MSG "Caracteres inválidos no mapa.\n");
                free(line);
                close(fd);
                return (1);
            }
            if (config_count < 6)
            {
                printf(ERROR_MSG "Configurações incompletas antes do mapa.\n");
                free(line);
                close(fd);
                return (1);
            }
            cfg->is_in_map_section = 1;
            if (map_line_count < MAX_MAP_LINES)
            {
                temp_map[map_line_count] = ft_strdup(line);
                if (!temp_map[map_line_count])
                {
                    printf(ERROR_MSG "Erro de alocação de memória.\n");
                    free(line);
                    close(fd);
                    return (1);
                }
                map_line_count++;
            }
            else
            {
                printf(ERROR_MSG "Mapa excede o limite de %d linhas.\n", MAX_MAP_LINES);
                free(line);
                close(fd);
                return (1);
            }
        }
        free(line);
    }
    close(fd);
    
    if (config_count < 6 || map_line_count == 0)
    {
        printf(ERROR_MSG "Configurações ou mapa incompletos.\n");
        return (1);
    }

    cfg->map = copy_map(temp_map, map_line_count);
    cfg->map_height = map_line_count;
    for (int i = 0; i < map_line_count; i++)
        free(temp_map[i]);
    
    if (validate_map(cfg))
    {
        free_config_and_map(cfg);
        return (1);
    }

    return (0);
}
