#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class miString : public std::string
{
	const miString readFile(const std::string& path)
	{
		return miString();
	}

	miString readLine()
	{
		return miString();
	}

	miString toLower()
	{
		return miString();
	}

	bool contains(miString text)
	{
		return false;
	}

	
};

