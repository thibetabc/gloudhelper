#ifndef __H_STRINGUTIL_H__
#define __H_STRINGUTIL_H__
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class StringUtil
{
public:
	//分割
	vector<string> split(string str, string pattern);
	//替换
	string &replace_all_distinct(string &str, const string &old_value, const string &new_value);
	//找出数字
	string get_all_number(string str);
};
#endif
