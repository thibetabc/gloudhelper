#include "XMLParser.h"
#include <fstream>
#include "cocos2d.h"


XMLParser* XMLParser::_getInstance = nullptr;

XMLParser::XMLParser()
{
}

XMLParser::~XMLParser()
{
	_strings.clear();
}

void XMLParser::destroyInstance()
{
	if (_getInstance)
	{
		delete _getInstance;
		_getInstance = nullptr;
	}

}
XMLParser *XMLParser::getInstance(std::string xmlFile)
{

	if (_getInstance == nullptr)
	{
		_getInstance = new XMLParser();
		if (!_getInstance->parseWithFile(xmlFile.c_str()))
		{
			delete _getInstance;
			_getInstance = nullptr;
		}
	}
	return _getInstance;
}

bool XMLParser::parseWithFile(const char * xmlFile)
{
    std::ifstream xmlraw(xmlFile);
    if (xmlraw.bad())
        return false;
    log("XMLParser::parseWithFile(...), called: xmlFile = %s", xmlFile);
    /*std::string content((std::istreambuf_iterator<char>(xmlraw)),
                        std::istreambuf_iterator<char>());*/

	std::string content = FileUtils::getInstance()->getStringFromFile(xmlFile);
    return parseWithString(content.c_str());
}

bool XMLParser::parseWithString(const char * content)
{
    _strings.clear();

	if (content == nullptr || strlen(content) <= 0)
	{
		return false;
	}

    SAXParser saxParser;
    saxParser.setDelegator(this);
    return saxParser.parse(content, strlen(content));
}

void XMLParser::startElement(void * ctx, const char * name, const char ** atts)
{
	_startElement = (char *)name;
	if (_startElement == "string")
	{
		while (atts && *atts)
		{
			const char *attsKey = *atts;
			if (0 == strcmp(attsKey, "name"))
			{
				++atts;
				const char *attsValue = *atts;
				_key = attsValue;
				break;
			}
			++atts;
		}
	}
}

void XMLParser::endElement(void * ctx, const char * name)
{
	_endElement = (char *)name;
}

void XMLParser::textHandler(void * ctx, const char * text, int len)
{
	if (_key.length() == 0)
		return;

    std::string value((char*)text, 0, len);

	bool noValue = true;
	for (unsigned int i = 0; i < value.length(); i++)
	{
		char c = value.at(i);
        if (c != ' ' && c != '\r' && c != '\n' && c != '\t')
		{
			noValue = false;
			break;
		}
	}
	if (noValue) 
        return;

    _strings[_key] = value;
}

std::string XMLParser::operator[](const std::string & key)
{
    return _strings[key];
}

std::string replace(std::string source, std::string srcPattern, std::string dstPattern)
{
    std::string result;
    std::string::size_type pos;
    unsigned int lenSource = source.length();
    unsigned int i = 0;

    for (i = 0; i < lenSource; ++i)
	{
        pos = source.find(srcPattern, i);
        if (std::string::npos == pos)
			break;

		if (pos < lenSource)
		{
            std::string s = source.substr(i, pos - i);
			result += s;
			result += dstPattern;
            i = pos + srcPattern.length() - 1;
		}
	}
	result += source.substr(i);

	return result;
}
