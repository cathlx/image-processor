#pragma once

#include <string>
#include <vector>

class Parser {
public:
    struct Filter {
        std::string name;
        std::vector<std::string> parameters;
    };

    struct Commands {
        std::string input_path;
        std::string output_path;
        std::vector<Filter> filters;
    };

    Parser() = default;
    Parser(int argc, char** argv);
    ~Parser() = default;

    void Parse(int argc, char** argv);
    void HelpParse(int argc, std::vector<std::string> args);

    Commands& Arguments();

private:
    Commands commands_;
};
