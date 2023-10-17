/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xmatute- <xmatute-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:21:55 by xmatute-          #+#    #+#             */
/*   Updated: 2023/10/17 17:40:29 by xmatute-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

#include "../templates.h"

#include "Server.hpp"
#include <vector>

typedef std::vector<Server> SVector;

#include <string>
using	std::string;

#include <fstream>
using	std::ifstream;

#include <iostream>
using	std::cerr;
using	std::endl;

class	Config
{
	private:
		//atributos

	private:
		//funciones

	private:
		Config();
		Config(const Config& to_copy);
		Config& operator=(const Config& to_asign);
		~Config();

	public:
		Config(const std::string &path);

		size_t			GetServersNum();

		string			getName(size_t index);
		string			getRoot(size_t index);
		intVector		getPorts(size_t index);
		intCharMap		getErrorPages(size_t index);
		locationVector	getLocations(size_t index);
};

#endif
