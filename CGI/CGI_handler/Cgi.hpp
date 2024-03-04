/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmohamed <hmohamed@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 12:42:45 by hmohamed          #+#    #+#             */
/*   Updated: 2024/03/04 12:26:54 by hmohamed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP


#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class Cgi {
	public:
    Cgi(char* filePath, char** arguments, char** environment);
	~Cgi();
    void execute();

private:
    char* filePath;
    char** arguments;
    char** environment;
	
};

#endif

