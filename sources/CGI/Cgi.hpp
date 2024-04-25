/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:42:45 by hmohamed          #+#    #+#             */
/*   Updated: 2024/04/25 13:04:14 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "Request.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "TimeOut.hpp"
#include "Server.hpp"
#include "signal.h"

class Cgi {
	public:
   // Cgi(char* filePath, char** arguments, char** environment);
        Cgi(const Request &request, const ServerConfig &config);
        ~Cgi();
        void execute(const std::string &outputFile);
        string execute(void);
		char **headersToEnv(const Request &request, const string res, const ServerConfig &config);
		TimeOut timer;

    private:
        vsp    headers;
        string queryString;
		string filePath;
		string body;
        char **arguments;
        char **environment;
};

#endif
