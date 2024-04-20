
// #include <iostream>
// # include <stdio.h>
// # include <unistd.h>
// # include <fcntl.h>
// # include <signal.h>
// # include <sys/stat.h>
// # include <sys/wait.h>
// # include <stdexcept>
#include "Cgi.hpp"


//using namespace webserv::http;


// int	main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	//(void)av;
// 	int		fd[2];
// 	int		id;

// 	pipe(fd);
// 	id = fork();
// 	if (id == 0)
// 	{
// 		//dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		if (execve("/bin/ls", ++av, env))
// 			write(2, "error\n", 6);
// 	}
// 	close(fd[0]);
// 	close(fd[1]);

// 	return (0);
// }



int main(int argc, char** argv, char** envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	vsp headers;

	// for (int i = 0; envp[i] != NULL; ++i)
    // {
    //     std::cout << envp[i] << std::endl;
    // }
	// cout<< endl<<endl;

	//const std::string outputFile;

	string res;

    try
    {
        Request request(sample_request_cgi);
        // headers = request.get_headers();
		// cout<< endl << request.get_resource()<< endl;

        // // Print headers
        // cout << "Headers:" << endl;
        // for (vsp::iterator it = headers.begin();
        //      it != headers.end(); ++it)
        // {
        //     cout << it->first << ": " << it->second << endl;
        // }

        Cgi     cgi(request);

		res = cgi.execute();
		cout << "this return exce CGI : " <<res << endl;

       //cgi.execute("stds");
    }
    catch (std::runtime_error &e)
    {
        cerr << e.what() << std::endl;
    }

    // const char *pythonScriptPath = "/Users/hashim/Desktop/42curses/webserv/CGI/tester/file.sh";
    // const char *pythonInterpreterPath = "/Users/hashim/Desktop/42curses/webserv/CGI/tester/file.sh";

    // // Check if the Python script exists
    // if (access(pythonScriptPath, X_OK) == -1)
    // {
    //     std::cerr << "Error: Python script not found or does not have execution permission." << std::endl;
    //     return 1;
    // }

    // // Create an instance of the CGI class
    // char* scriptArguments[] = { const_cast<char*>(pythonScriptPath), nullptr };
    // Cgi cgi(const_cast<char*>(pythonInterpreterPath), scriptArguments, envp);

    // // Execute the Python script
    // cgi.execute("std");
    return 0;
}