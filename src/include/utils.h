#ifndef __UTILS__
#define __UTILS__

#include <vector>
#include <string>
#include <sstream>

namespace Utils {
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	    	if (!item.empty())
	        	elems.push_back(item);
	    }
	    return elems;
	}


	std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}
}

#endif