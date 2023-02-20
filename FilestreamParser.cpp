#include "FilestreamParser.h"
#include <sstream>

std::string GetFileString(const std::string& path)
{
	std::ifstream stream(path);
	std::string outputLine = "";
	std::string currentLine = "";
	while(std::getline(stream, currentLine))
	{
		outputLine += currentLine + '\n';
	}
	return outputLine;
}