#ifndef _XMLPARSE_H_
#define _XMLPARSE_H_

#include <map>
#include "platform/CCSAXParser.h"
USING_NS_CC;

class XMLParser : public SAXDelegator
{
public:
    XMLParser();
	~XMLParser();

	static XMLParser *getInstance(std::string xmlFile);
	static void destroyInstance();
	bool parseWithFile(const char * xmlFile);
	bool parseWithString(const char * content);
    std::string operator[](const std::string & key);

protected:
    void startElement(void * ctx, const char * name, const char ** atts);
    void endElement(void * ctx, const char * name);
    void textHandler(void * ctx, const char * text, int len);

private:
	std::map<std::string, std::string> _strings;

    // temporary;
	std::string _key;
	std::string _startElement;
	std::string _endElement;
	static XMLParser* _getInstance;

};

#endif
