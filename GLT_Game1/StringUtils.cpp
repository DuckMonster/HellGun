#include "StringUtils.h"

#include <sstream>

using namespace std;

vector<string> split(std::string input, char denominator) {
	vector<string> output;
	stringstream ss(input);

	string str;

	while (getline(ss, str, denominator)) {
		//Don't add empty strings
		if (!str.empty())
			output.push_back(str);
	}

	return output;
}


string trim(string s, string trimValues) {
	for (int i = 0; i < s.length(); i++)
		if (trimValues.find(s[i]) == string::npos) {
			s.erase(s.begin(), s.begin() + i);
			break;
		}

	for (int i = s.length() - 1; i >= 0; i++)
		if (trimValues.find(s[i]) == string::npos) {
			s.erase(s.begin() + i, s.begin() + s.length() - 1);
			break;
		}

	return s;
}