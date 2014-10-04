#ifndef __UTILS__
#define __UTILS__

#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace Utils {
	vector<string> &split(const string &s, char delim, vector<string> &elems) {
	    stringstream ss(s);
	    string item;
	    while (getline(ss, item, delim)) {
	    	if (!item.empty())
	        	elems.push_back(item);
	    }
	    return elems;
	}


	vector<string> split(const string &s, char delim) {
	    vector<string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	/**
	 * Parse (int, int) string into pair<int,int>
	 */
	pair<int, int> parseIntPair(string intPair) {

		// replace with space
		for (int i = 0; i < intPair.size(); i++) {
			if (intPair[i] == '(' || intPair[i] == ')' || intPair[i] == ',')
				intPair[i] = ' ';
		}

		istringstream iss(intPair);

		int f = 0, s = 0;

		iss >> f >> s;

		return pair<int,int>(f,s);
	}
}

#endif