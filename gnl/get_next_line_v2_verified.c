#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "get_next_line.h"

char    *ft_join(char *line, char *buffer, int size, int from, int to)
{
    int     i;
    char    *res;
    i = 0;
    if(!line)
        return (NULL);
    res = malloc(sizeof(char) * (size + to + 1));
    if (!res)
    {
        free(line);
        return (NULL);
    }
    while (i < size)
    {
        res[i] = line[i];
        i++;
    }
    while (from < to)
    {
        res[i] = buffer[from];
        i++;
        from++;
    }
    res[i] = '\0';
    free(line);
    return (res);
}
char *get_next_line(int fd)
{
    int     start;
    int     size;
    char    *line;
    static  int cursor = 0;
    static  int bytes_read = -1;
    static  char buffer[BUFFER_SIZE];

    if (fd < 0 || BUFFER_SIZE < 1)
        return (NULL);
    if (cursor >= bytes_read)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        cursor = 0;
    }
    if (!bytes_read || bytes_read == -1)
    {
        cursor = 0;
        return (NULL);
    }
    line = malloc(1);
    size = 0;
    while(bytes_read > 0 && line)
    {
        start = cursor;
        while(cursor < bytes_read && buffer[cursor] != '\n')
            cursor++;
        if (cursor >= bytes_read)
        {
            line = ft_join(line, buffer, size, start, cursor);
            bytes_read = read(fd, buffer, BUFFER_SIZE);
            size += (cursor - start);
            cursor = 0;
        }
        else if (buffer[cursor] == '\n')
        {
            cursor++;
            return (ft_join(line, buffer, size, start, cursor));
        }
    }
    if (bytes_read == -1)
    {
        if (line)
            free(line);
        return (NULL);
    }
    return (line);
}

int main()
{
	int 	fd;
	char	*line;

	fd = open("text", O_RDONLY);
	while ((line = get_next_line(fd)) != NULL) {
		printf("%s", line);
		free(line);
	}
	close(fd);
	printf("\n");
	return 0;
}
