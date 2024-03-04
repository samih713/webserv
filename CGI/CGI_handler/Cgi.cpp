/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:44:51 by hmohamed          #+#    #+#             */
/*   Updated: 2024/03/04 12:27:05 by hmohamed         ###   ########.fr       */
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

    pipe(fd);
    id = fork();
    if (id == 0)
    {
        // Child process
        close(fd[0]);
        close(fd[1]);

        if (execve(filePath, arguments, environment) == -1)
        {
            std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
            _exit(EXIT_FAILURE);
        }
    }
    close(fd[0]);
    close(fd[1]);

    // Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);
}