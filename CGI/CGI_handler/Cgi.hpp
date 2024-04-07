/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:42:45 by hmohamed          #+#    #+#             */
/*   Updated: 2024/04/06 04:41:25 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

//#include "../../includes/webserv.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "webserv.hpp"
#include "../sources/http/request/Request.hpp"

class Cgi {
	public:
   // Cgi(char* filePath, char** arguments, char** environment);
        Cgi(const Request &request);
        ~Cgi();
        void execute(const std::string &outputFile);

    private:
        vsp    headers;
        string queryString;
		string filePath;
        char **arguments;
        char **environment;
	
};

#endif

