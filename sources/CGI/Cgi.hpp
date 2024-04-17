/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:42:45 by hmohamed          #+#    #+#             */
/*   Updated: 2024/04/16 01:44:42 by hmohamed         ###   ########.fr       */
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
        Cgi(const Request &request);
        ~Cgi();
        void execute(const std::string &outputFile);
        string execute(void);

    private:
        vsp    headers;
        string queryString;
		string filePath;
        char **arguments;
        char **environment;
	
};

#endif

