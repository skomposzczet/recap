#include "parser_builder.hpp"
#include "positional_arg.hpp"
#include "flag.hpp"
#include "arg.hpp"
#include <iostream>

using namespace rcp;

Parser build_parser() {
    Parser parser = ParserBuilder("demo")
        .brief("example app to present library")
        .description("Long description about app")
        .epilog("Some summary")
        .author("skomposzczet")
        .help_enabled()
        .get();
    return parser;
}

void add_args(Parser& parser) {
    parser.add_positional_argument(
        PositionalArgBuilder("pos_arg")
            .required()
            .with_description("this is first positional arg, it is mandatory")
            .get()
    );
    parser.add_positional_argument(
        PositionalArgBuilder("optional_pos_arg")
            .with_description("this is second positional arg, it is not mandatory")
            .get()
    );
    parser.add_flag(
        FlagBuilder("flag")
            .with_description("this is flag arg, acts like switch, is either on or off")
            .get()
    );
    parser.add_argument(
        ArgBuilder("arg")
            .default_value("def_value")
            .with_alias("alias")
            .with_description("first named arg, has default value and can also be used with alias")
            .get()
    );
    parser.add_argument(
        ArgBuilder("cond_arg")
            .with_condition(Condition::int_parsable())
            .with_description("second named arg, parser will only accept its value if it meets condition")
            .get()
    );
}

void print_values(const Parser& parser) {
    std::cout << "Flag was";
    if (!parser.was_called("flag"))
        std::cout << " not";
    std::cout << " toggled\n";

    std::cout << "Argument with condition was";
    auto res = parser.get("cond_arg");
    if (res.has_value())
        std::cout << " successfully parsed as " << std::stoi(res.value()) << "\n";
    else
        std::cout << " not supplied\n";
}

int main(int argc, const char* argv[]) {
    auto parser = build_parser();
    add_args(parser);
    try {
        parser.parse(argc, argv);
    } catch(const RcpError& e) {
        std::cerr << "Exception caught:\n" << e.what() << "\n";
        return 1;
    }
    
    if (parser.help_triggered()) {
        parser.help().to_screen();
        return 0;
    }

    print_values(parser);
}