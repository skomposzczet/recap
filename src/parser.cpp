#include "parser.hpp"
#include "rcp_error.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace rcp {

std::unique_ptr<ParserBuilder> Parser::build(const std::string& app_name) {
    return std::make_unique<ParserBuilder>(app_name);
}

void Parser::parse(int argc, char** argv) {
    if (argc % 2 == 0)
        throw ParseError(std::string{"Invalid number of args: "} + std::to_string(argc));

    for (int i = 1 ; i < argc ; ++i) {
        auto option = extract_option(argv[i++]);
        auto& arg = get_arg_by_option(option);
        arg->set(argv[i]);
    }
}

std::string Parser::extract_option(const std::string& str) {
    std::vector<std::string> prefixes{"--", "-"};

    for (const auto& prefix: prefixes) {
        if (str.rfind(prefix) != std::string::npos) {
            std::string option = std::string(str.begin()+prefix.size(), str.end());
            if (!option.empty())
                return option;
        }
    }

    throw ParseError(std::string{"Unexpected item: "} + str + "; option should start with hyphen");
}

ArgsVecType::value_type& Parser::get_arg_by_option(const std::string& option) {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        throw ParseError(std::string{"Unregistered option: "} + option);

    return *res;
}

void Parser::add_argument(ArgsVecType::value_type arg) {
    args.push_back(arg);
}

OptionValue Parser::get(const std::string& option) const {
    auto res = std::find_if(args.begin(), args.end(), [&option](const auto& arg){
        return arg->is_triggered(option);
    });

    if (res == args.end())
        throw ParseError(std::string{"Unregistered option: "} + option);

    return (*res)->get();
}

std::string Parser::help() { 
    std::string help_text = std::string{"\t"} + name + " - " + brief
        + "\n\n\t" + description + "\n\n";

    for (auto& arg: args) {
        help_text += std::string{"\t\t"} + arg->help() + "\n";
    }

    help_text += authors_help_str();

    return help_text;
}

std::string Parser::authors_help_str() const {
    if (authors.empty())
        return std::string{""};

    std::string begin{"\n\n\t"};
    if (authors.size() == 1u) 
        return begin + "author: " + authors.at(0) + "\n";
    
    begin += "authors: ";
    for (const std::string& author: authors)
        begin += author + ", ";
    return begin + "\b\b \n";
}

ParserBuilder::ParserBuilder(const std::string& name) {
    parser.name = name;
}

ParserBuilder& ParserBuilder::description(const std::string& description) {
    parser.description = description;
    return *this;
}

ParserBuilder& ParserBuilder::brief(const std::string& brief) {
    parser.brief = brief;
    return *this;
}

ParserBuilder& ParserBuilder::epilog(const std::string& epilog) {
    parser.epilog = epilog;
    return *this;
}

ParserBuilder& ParserBuilder::version(double version) {
    parser.version = version;
    return *this;
}

ParserBuilder& ParserBuilder::author(const std::string& new_author) {
    parser.authors.push_back(new_author);
    return *this;
}

ParserBuilder& ParserBuilder::authors(std::initializer_list<std::string> authors) {
    parser.authors.insert(parser.authors.end(), authors.begin(), authors.end());
    return *this;
}

Parser ParserBuilder::get() const {
    return parser;
}

}
