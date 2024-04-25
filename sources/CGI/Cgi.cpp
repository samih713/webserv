/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:44:51 by hmohamed          #+#    #+#             */
/*   Updated: 2024/04/25 14:04:56 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Cgi.hpp"

string getStingQuery(string res)
{
	string resn;
	size_t qu;
	size_t length;

	qu = res.find('?', 0); 
	length = res.length();
	cout<< "the ?  :" << qu <<endl;
    cout << "lingth:" << length << endl;
    resn = res.substr(qu + 1, length);
	//result = const_cast<char *>(res.substr(0,qu).c_str());
	cout << resn << endl;
    return (resn);
}


string geturi(string res)
{
	string resn;
	size_t qu;

	qu = res.find('?', 0); 
	resn = res.substr(0, qu);
	//result = const_cast<char *>(res.substr(0,qu).c_str());
	cout << resn << endl;
	cout<< "test" << qu <<endl;
    return (resn);
}

Cgi::Cgi(const Request &request, const ServerConfig &config): body(request.get_body())
{
	string res;

	(void)config;
	res = const_cast<char *>(request.get_resource().c_str());
	filePath = (geturi(res));
	headers = request.get_headers();
	environment = headersToEnv(request, res, config);
	timer = request.timer;
	//filePath = const_cast<char *>(geturi(request.get_resource())->c_str());
	//queryString = getStingQuery(res);
    //filePath = const_cast<char *> (request.get_resource().c_str());
    // // Check if the Python script exists
    // if (access(filePath, X_OK) == -1)
    // {
    //     std::cerr << "Error: Python script not found or does not have execution permission." << std::endl; 
	// 	return ;
    // }
   	cout<< "file path :" << filePath << endl;
    arguments = new char *[2];
	arguments[0] = const_cast<char *>(filePath.c_str());
	arguments[1] = NULL;

    // Print out the environment variables
    for (int i = 0; environment[i] != NULL; ++i)
    {
        std::cout << environment[i] << std::endl;
    }
}

Cgi::~Cgi()
{
    for (int i = 0; environment[i] != NULL; ++i)
        delete[] environment[i];
    delete[] environment;
    delete[] arguments;
}

char ** Cgi::headersToEnv(const Request &request, const string res, const ServerConfig &config)
{
    std::vector<char *> envVector;

	(void)config;
	(void)request;

	string queryString = getStingQuery(res);;
    // Iterate through headers
    for (vsp::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        size_t len = it->first.size() + it->second.size() + 2;
        char *envEntry = new char[len];
        std::snprintf(envEntry, len, "%s=%s", it->first.c_str(), it->second.c_str());
        envVector.push_back(envEntry);
    }

    // std::string gateway_interface = "GATEWAY_INTERFACE=CGI/1.1";
    // envVector.push_back(const_cast<char *>(gateway_interface.c_str()));

	envVector.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    envVector.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	string val;
	val = "SERVER_NAME=" + config.serverName;
	envVector.push_back(strdup(val.c_str()));
	val = "SERVER_SOFTWARE=" + config.serverName + "/1.0";
	envVector.push_back(strdup(val.c_str()));
	val = "SERVER_PORT=" + std::to_string(config.listenerPort);
	envVector.push_back(strdup(val.c_str()));
	val = "REQUEST_URI=" + request.get_resource();
	envVector.push_back(strdup(val.c_str()));
	queryString = getStingQuery(res);
	// val = "QUERY_STRING=" + queryString;
	// envVector.push_back(strdup(val.c_str()));
    // "REQUEST_METHOD"
	// "PATH_INFO"
	// "PATH_TRANSLATED"
	// "QUERY_STRING"
	// "REMOTEaddr"
	// "REMOTE_IDENT"
	// "REMOTE_USER"

	
	char* token = strtok(const_cast<char *>(queryString.c_str()), "&");
    
	//cout << token +3 <<endl;
    // Iterate through the tokens and push integers onto the stack
    while (token != NULL) {
		envVector.push_back(strdup(token));
        token = strtok(NULL, "&");
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


void Cgi::execute(const std::string& outputFile)
{
    int         fd[2];
    int         id;
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
    }
    else if (id == 0) {
        // Child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(const_cast<char*>(filePath.c_str()), arguments, environment) == -1) {
            std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
            _exit(EXIT_FAILURE);
        }
    }

    // Parent process
    close(fd[1]); // Close the write end of the pipe

    // Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);

    // Read the response from the pipe
    char    buffer[91];
    ssize_t bytesRead;
    while ((bytesRead = read(fd[0], buffer, 90)) > 0)
        res_body.append(buffer, bytesRead);
    // Save the response to a file
    std::ofstream outFile(outputFile.c_str());
    if (outFile.is_open()) {
        outFile << res_body;
        outFile.close();
        std::cout << "Response saved to: " << outputFile << std::endl;
    }
    else {
        std::cerr << "Error opening output file: " << strerror(errno) << std::endl;
    }

    close(fd[0]);
}


string Cgi::execute(void)
{
    int         fd[2];
    int         id;
    std::string res_body;

    // Create a pipe for communication
    if (pipe(fd) == -1) {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
        return NULL;
    }
	cout<< "FBB" << filePath << endl;

    // Fork the process
    // TODO fix forking here for sleep
    id = fork();
    if (id == -1) {
        std::cerr << "Error forking process: " << strerror(errno) << std::endl;
        return NULL;
    }
    else if (id == 0) {
        // Child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(const_cast<char*>(filePath.c_str()), arguments, environment) == -1) {
            std::cerr << "Error executing execve: " << strerror(errno) << std::endl;
            _exit(EXIT_FAILURE);
        }
    }

    // Parent process
    close(fd[1]); // Close the write end of the pipe	
	int status;
	// Wait for the child process to finish or until timeout
    while (true) {
        // Check if the timeout duration has been reached
        if (timer.is_timeout()) {
            std::cerr << "Timeout reached. Exiting." << std::endl;
            // Terminate the child process
            kill(id, SIGTERM);
            break;
        }

        // Check if the child process has finished
        if (waitpid(id, &status, WNOHANG) == id) {
            // Child process has finished
			// Read from the pipe
	        char buffer[1024];
	        ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer));
	        if (bytesRead > 0) {
	            res_body.append(buffer, bytesRead);
	        } else if (bytesRead == 0) {
	            break;
	        } else {
	            // Error while reading from the pipe
	            if (errno == EAGAIN || errno == EWOULDBLOCK) {
	                // No data available yet, continue waiting
	                usleep(100000);
	                continue;
	            } else {
	                std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
	                break;
	            }
	        }
            break;
        }
         usleep(100000);
    }
	

    close(fd[0]);
    return (res_body);
}
