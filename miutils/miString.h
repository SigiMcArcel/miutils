#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class miString : public std::string
{
	miString readFile(const std::string& path)
	{
		std::ifstream input_file(path);
		if (!input_file.is_open()) {
			return miString();
		}
		return miString((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	}

	miString readLine()
	{

	}

	miString toLower()
	{

	}

	bool contains(miString text)
	{
		return false;
	}

	std::vector<miString> split(const std::vector<miString>)
	{

	}
};

