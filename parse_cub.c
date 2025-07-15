/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:01 by lalves-d          #+#    #+#             */
/*   Updated: 2025/07/15 15:58:02 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_color(const char *line)
{
	int		r;
	int		g;
	int		b;
	char	*c;
	char	*parts[3];
	int		count;

	c = (char *)line;
	parts[0] = c;
	parts[1] = NULL;
	parts[2] = NULL;
	count = 1;
	while (*c && count < 3)
	{
		if (*c == ',')
		{
			*c = '\0';
			parts[count++] = c + 1;
		}
		c++;
	}
	if (count != 3 || strchr(parts[2], ','))
		return (printf(ERROR_MSG "Formato de cor inválido: %s\n", line), -1);
	r = atoi(parts[0]);
	g = atoi(parts[1]);
	b = atoi(parts[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (printf(ERROR_MSG "Cor fora do intervalo [0,255]\n"), -1);
	return ((r << 16) | (g << 8) | b);
}

//Lógica de `is_map_line` para ser mais robusta
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
            return (0);
        if (line[i] == '1' || line[i] == '0')
            has_map_char = 1;
        i++;
    }
    return (has_map_char);
}

// Função de parsing agora libera memória antiga para evitar leaks
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
            printf("Aviso: Textura duplicada, usando a última.\n");
            free(*target_path);
        }
        char *path_start = line + 3;
        while (*path_start == ' ') path_start++;
        *target_path = ft_strdup(path_start);
        int len = strlen(*target_path);
        if (len > 0 && (*target_path)[len - 1] == '\n')
            (*target_path)[len - 1] = '\0';
    }
    return (0);
}

//Função principal de parsing com tratamento de erros e limpeza de memória
int parse_cub_file(char *filename, t_config *cfg)
{
    int     fd;
    char    *line;
    char    *temp_map[MAX_MAP_LINES];
    int     map_line_count = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (printf(ERROR_MSG "Não foi possível abrir o arquivo do mapa.\n"), 1);
    while ((line = get_next_line(fd)) != NULL)
    {
        char *trimmed = line;
        while (*trimmed == ' ')
            trimmed++; 
        if (starts_with(trimmed, "NO ") || starts_with(trimmed, "SO ") || 
            starts_with(trimmed, "WE ") || starts_with(trimmed, "EA "))
            parse_config_line(trimmed, cfg);
        else if (starts_with(trimmed, "F "))
            cfg->floor_color = parse_color(trimmed + 2);
        else if (starts_with(trimmed, "C "))
            cfg->ceiling_color = parse_color(trimmed + 2);
        else if (is_map_line(trimmed))
        {
            if (map_line_count < MAX_MAP_LINES)
                temp_map[map_line_count++] = ft_strdup(line); // Armazena a linha original
            else
                printf("Aviso: Mapa excede o limite de %d linhas.\n", MAX_MAP_LINES);
        }
        free(line);
    }
    close(fd);    
    if (!cfg->no_path || !cfg->so_path || !cfg->we_path || !cfg->ea_path ||
        cfg->floor_color == -1 || cfg->ceiling_color == -1 || map_line_count == 0)
        return (printf(ERROR_MSG "Configurações do mapa incompletas ou inválidas.\n"), 1);
    cfg->map = copy_map(temp_map, map_line_count);
    cfg->map_height = map_line_count;
    for (int i = 0; i < map_line_count; i++)
        free(temp_map[i]);
    return (0);
}
