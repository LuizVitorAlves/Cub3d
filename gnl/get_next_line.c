/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:52:42 by uviana-b          #+#    #+#             */
/*   Updated: 2024/11/24 14:58:22 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*my_reader(char *buffer, int fd)
{
	char	*reader;
	int		count_num;

	reader = NULL;
	count_num = 1;
	while (!ft_strchr(reader, '\n') && count_num > 0)
	{
		if (buffer[0] != '\0')
			reader = ft_strjoin(reader, buffer);
		if (ft_strlen(buffer) <= 0)
		{
			count_num = read(fd, buffer, BUFFER_SIZE);
			buffer[count_num] = '\0';
		}
	}
	if (ft_strlen(reader) == 0)
		return (NULL);
	return (reader);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*print;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	print = my_reader(buffer, fd);
	return (print);
}
/*
 #include <fcntl.h>
 #include  <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>

 int main(void)
 {
 //	int fd = open("files/42_with_nl", O_RDONLY);
 	int fd = open("gnlTester/files/42_with_nl", O_RDONLY);
 	char *line;
	
	int i = 0;
 	if (fd < 0)
 		return (1);
 	while (i < 3)
 	{
 		//printf("%s", line);
 		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		i++;
 	}
 	//printf("%s", line);
 	close(fd);
 	return (0);
}*/
