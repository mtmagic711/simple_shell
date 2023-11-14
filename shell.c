#include "main.h"

/**
* _strcmp - compare two strings.
* @s1: the first one.
* @s2: the second one.
* Return: 0, or > 0, or < 0.
*/

int _strcmp(const char *s1, const char *s2)
{
	int i = 0;

	while (*(s1 + i) != '\0')
	{
		if (*(s1 + i) != *(s2 + i))
		{
			return (*(s1 + i) - *(s2 + i));
		}
		i++;
	}
	return (0);
}

/**
* main - shell with arguments.
* @ac: arguments counts
* @av: array of strings.
* Return: Always 0.
*/

int main(int ac, char *av[])
{
	char *line = NULL, **args, *token;
	size_t len = 0;
	ssize_t read;
	int i = 0, st, pid;
	(void)ac;

	if (isatty(STDIN_FILENO) == 1)
		write(1, "$ ", 2);
	while ((read = getline(&line, &len, stdin)) != -1)
	{
		if (read == -1)
		{
			perror("geline");
			free(line), line = NULL;
			exit(EXIT_FAILURE);
		}
		if (read == 0)
		{
			free(line);
			continue;
		}
		if (_strcmp(line, "exit\n") == 0)
		{
			free(line), line = NULL;
			break;
		}
		token = strtok(line, " \t\n");
		if (token == NULL)
		{
			free(line), line = NULL;
			if (isatty(STDIN_FILENO) == 1)
				write(1, "$ ", 2);
			continue;
		}
		else
		{
			args = (char **)malloc((len + 2) * sizeof(char *));
			if (args == NULL)
			{
				perror("malloc");
				free(line), line = NULL;
				exit(EXIT_FAILURE);
			}
			while (token != NULL)
			{
				args[i] = token;
				i++;
				token = strtok(NULL, " \t\n");
			}
			args[i] = NULL;
			pid = fork();
			if (pid < 0)
			{
				perror("fork");
				free(args), args = NULL;
				free(line), line = NULL;
				exit(EXIT_FAILURE);
			}
			if (pid == 0)
			{
				if (execve(args[0], args, environ) == -1)
				{
					perror(av[0]);
					exit(127);
				}
			}
			else if (pid > 0)
			{
				wait(&st);
				if (isatty(STDIN_FILENO) == 1)
					write(1, "$ ", 2);
				i = 0;
				free(args), args = NULL;
			}
		}
	}
	free(line);
	free(args);
	return (0);
}
