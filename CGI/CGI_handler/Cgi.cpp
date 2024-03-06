/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:44:51 by hmohamed          #+#    #+#             */
/*   Updated: 2024/03/06 15:24:45 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Cgi.hpp"


Cgi::Cgi(char* filePath, char** arguments, char** environment): filePath(filePath), arguments(arguments), environment(environment)
{
	
}

Cgi::~Cgi()
{

}

void Cgi::execute()
{
    int fd[2];
    int id;
	char *res_body;
	int a;

	res_body = (char *)malloc(100);
	res_body[99] = '\0';
    pipe(fd);
    id = fork();
    if (id == 0)
    {
        // Child process
		dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(filePath, arguments, environment) == -1)
        {
            std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
            _exit(EXIT_FAILURE);
        }
    }
	
	// Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);
	a = read(fd[0], res_body, 90);
	res_body[a] = '\0';
	cout << res_body << endl;
    close(fd[0]);
    close(fd[1]);

    
}