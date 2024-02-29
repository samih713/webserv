
#include <iostream>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdexcept>


//using namespace webserv::http;


int	main(int ac, char **av, char **env)
{
	(void)ac;
	//(void)av;
	int		fd[2];
	int		id;

	pipe(fd);
	id = fork();
	if (id == 0)
	{
		//dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (execve("/bin/ls", ++av, env))
			write(2, "error\n", 6);
	}
	close(fd[0]);
	close(fd[1]);

	return (0);
}