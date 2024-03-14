/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:42:45 by hmohamed          #+#    #+#             */
/*   Updated: 2024/03/14 02:14:09 by hmohamed         ###   ########.fr       */
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
#include "Request.hpp"
using namespace webserv::http;

class Cgi {
	public:
   // Cgi(char* filePath, char** arguments, char** environment);
        Cgi(const webserv::http::Request &request);
        ~Cgi();
        void execute(const std::string &outputFile);

    private:
        vsp    headers;
        char  *filePath;
        char **arguments;
        char **environment;
	
};

#endif

