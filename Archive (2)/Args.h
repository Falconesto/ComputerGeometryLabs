//
// Created by Nikolay Rulev on 17.03.2020.
//

#ifndef INC_2_ARGS_H
#define INC_2_ARGS_H

#include <string>
#include <exception>
#include <stdexcept>

class Args {
public:
    Args(int argc, char **argv);
    [[nodiscard]] const std::string& input_file() const;
    [[nodiscard]] const std::string& output_file() const;
    [[nodiscard]] const std::string& from() const;
    [[nodiscard]] const std::string& to() const;
    [[nodiscard]] int input_count() const;
    [[nodiscard]] int output_count() const;
private:
    std::string input_file_;
    std::string output_file_;
    std::string from_;
    std::string to_;
    int input_count_;
    int output_count_;
};


#endif //INC_2_ARGS_H
