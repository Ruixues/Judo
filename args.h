#pragma once

#include "thirdparty/argparse/argparse.hpp"
#include <string_view>
#include "config.h"
#include "core.h"

void RunJudoProgram(std::string in);

void HandleInput(int argc, char *argv[])
{
    argparse::ArgumentParser program("Judo", JudoVersion);
    program.add_argument("-version")
        .help("show the version of Judo")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("-i")
        .help("specialize the Judo source file")
        .default_value("");
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }
    if (program["-version"] == true)
    {
        std::cout << "Judo " + JudoVersion << std::endl;
        std::cout << "Compiled at " << __TIMESTAMP__ << std::endl;
    }
    std::string input = program.get<std::string>("-i");
    if (input != "")
    {
        //开始加载
        RunJudoProgram(input);
    }
}

void RunJudoProgram(std::string in)
{
    auto Engine = std::make_shared<Judo>(in);
}