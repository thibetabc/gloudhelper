#include "StringUtil.h"

//分割
vector< string> StringUtil::split(string str, string pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}
//替换
string &StringUtil::replace_all_distinct(string &str, const string &old_value, const string &new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())   {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}
//找出数字
string StringUtil::get_all_number(string str)
{
	string result = "";
	string numberics = "0123456789";
	string::size_type pos = 0;
	while ((pos = str.find_first_of(numberics, pos)) != string::npos)
	{
		result += str[pos++];
		//cout<<"str{"<<pos<<"}="<<str[pos++]<<endl;
	}
	return result;
}