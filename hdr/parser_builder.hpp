#ifndef _RCP_PARSER_BUILDER_H_
#define _RCP_PARSER_BUILDER_H_

#include "parser.hpp"
#include <string>

namespace rcp {

class ParserBuilder
{
public:
    ParserBuilder(const std::string& name);

    ParserBuilder& description(const std::string& description);
    ParserBuilder& brief(const std::string& brief);
    ParserBuilder& epilog(const std::string& epilog);
    ParserBuilder& version(const std::string& version);
    ParserBuilder& author(const std::string& new_author);
    ParserBuilder& authors(std::initializer_list<std::string> authors);

    ParserBuilder& help_enabled();    
    ParserBuilder& version_enabled();    

    ParserBuilder& ignore_required_on_help();

    Parser get() const;

private:
    Parser parser;

};

}

#endif // _RCP_PARSER_BUILDER_H_
