#ifndef __TANKGAME_UTILS__
#define __TANKGAME_UTILS__

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

namespace Utils {
	vector<string>& split(const string &s, char delim, vector<string> &elems);

	vector<string> split(const string &s, char delim);

	/**
	 * Parse (int, int) string into pair<int,int>
	 */
	pair<int, int> parseIntPair(string intPair);

	int parseInt(string intNum);
}

#endif