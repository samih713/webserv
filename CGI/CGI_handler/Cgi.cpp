/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:44:51 by hmohamed          #+#    #+#             */
/*   Updated: 2024/03/17 04:22:38 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Cgi.hpp"


// Cgi::Cgi(char* filePath, char** arguments, char** environment): filePath(filePath), arguments(arguments), environment(environment)
// {
	
// }

char **headersToEnv(vsp &headers)
{
    std::vector<char *> envVector;

    // Iterate through headers
    for (vsp::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        size_t len = it->first.size() + it->second.size() + 2;
        char *envEntry = new char[len];
        std::snprintf(envEntry, len, "%s=%s", it->first.c_str(), it->second.c_str());
        envVector.push_back(envEntry);
    }

    // Allocate memory for char* array
    char **envp = new char *[envVector.size() + 1]; 

    // Copy pointers from vector to char* array
    for (size_t i = 0; i < envVector.size(); ++i)
    {
        envp[i] = envVector[i];
    }
    envp[envVector.size()] = NULL;

    return envp;
}


Cgi::Cgi(const Request &request)
{
	headers = request.get_headers();
	environment = headersToEnv(headers);
    filePath = const_cast<char *> (request.get_resource().c_str());
    // // Check if the Python script exists
    if (access(filePath, X_OK) == -1)
    {
        std::cerr << "Error: Python script not found or does not have execution permission." << std::endl; 
		return ;
    }
    arguments = new char *[2];
	arguments[0] = filePath;
	arguments[1] = NULL;

    cout<<filePath << endl;
    // Print out the environment variables
    for (int i = 0; environment[i] != NULL; ++i)
    {
        std::cout << environment[i] << std::endl;
    }
}

Cgi::~Cgi()
{
    for (int i = 0; environment[i] != NULL; ++i)
    {
        delete[] environment[i];
    }
    delete[] environment;
	delete[] arguments;
}

// void Cgi::execute()
// {
//     int fd[2];
//     int id;
// 	char *res_body;
// 	int a;

// 	res_body = (char *)malloc(100);
// 	res_body[99] = '\0';
//     pipe(fd);
//     id = fork();
//     if (id == 0)
//     {
//         // Child process
// 		dup2(fd[1], STDOUT_FILENO);
//         close(fd[0]);
//         close(fd[1]);

//         if (execve(filePath, arguments, environment) == -1)
//         {
//             std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
//             _exit(EXIT_FAILURE);
//         }
//     }
	
// 	// Wait for the child process to finish
//     int status;
//     waitpid(id, &status, 0);
// 	a = read(fd[0], res_body, 90);
// 	res_body[a] = '\0';
// 	cout << res_body << endl;
//     close(fd[0]);
//     close(fd[1]);

    
// }


void Cgi::execute(const std::string& outputFile)
{
    int fd[2];
    int id;
    std::string res_body;

    // Create a pipe for communication
    if (pipe(fd) == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        return;
    }

    // Fork the process
    id = fork();
    if (id == -1) {
        std::cerr << "Error forking process: " << strerror(errno) << std::endl;
        return;
    } else if (id == 0) {
        // Child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(filePath, arguments, environment) == -1) {
            std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
            _exit(EXIT_FAILURE);
        }
    }

    // Parent process
    close(fd[1]);  // Close the write end of the pipe

    // Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);

    // Read the response from the pipe
    char buffer[91];
    ssize_t bytesRead;
    while ((bytesRead = read(fd[0], buffer, 90)) > 0) {
        res_body.append(buffer, bytesRead);
    }
    // Save the response to a file
    std::ofstream outFile(outputFile);
    if (outFile.is_open()) {
        outFile << res_body;
        outFile.close();
        std::cout << "Response saved to: " << outputFile << std::endl;
    } else {
        std::cerr << "Error opening output file: " << strerror(errno) << std::endl;
    }

    close(fd[0]);
}

// std::string Cgi::execute(void)
// {
//     int fd[2];
//     int id;
//     std::string res_body;

//     // Create a pipe for communication
//     if (pipe(fd) == -1) {
//         std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
//         return (NULL);
//     }

//     // Fork the process
//     id = fork();
//     if (id == -1) {
//         std::cerr << "Error forking process: " << strerror(errno) << std::endl;
//         return (NULL);
//     } else if (id == 0) {
//         // Child process
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[0]);
//         close(fd[1]);

//         if (execve(filePath, arguments, environment) == -1) {
//             std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
//             _exit(EXIT_FAILURE);
//         }
//     }

//     // Parent process
//     close(fd[1]);  // Close the write end of the pipe

//     // Wait for the child process to finish
//     int status;
//     waitpid(id, &status, 0);

//     // Read the response from the pipe
//     char buffer[91];
//     ssize_t bytesRead;
//     while ((bytesRead = read(fd[0], buffer, 90)) > 0) {
//         res_body.append(buffer, bytesRead);
//     }
//     // Save the response to a file
//     // std::ofstream outFile(outputFile);
//     // if (outFile.is_open()) {
//     //     outFile << res_body;
//     //     outFile.close();
//     //     std::cout << "Response saved to: " << outputFile << std::endl;
//     // } else {
//     //     std::cerr << "Error opening output file: " << strerror(errno) << std::endl;
//     // }

//     close(fd[0]);
// 	return(res_body);
// }