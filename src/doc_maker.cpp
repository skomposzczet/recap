#include "doc_maker.hpp"
#include "util.hpp"
#include <algorithm>
#include <iostream>

namespace rcp {

char get_arg_info_sort_letter(const ArgInfo& info) {
    if (info.short_version.has_value())
        return info.short_version.value().at(0);
    return info.long_version.value().at(0);
}

bool arg_info_comp(const ArgInfo& i1, const ArgInfo& i2) {
    char i1l = get_arg_info_sort_letter(i1);
    char i2l = get_arg_info_sort_letter(i2);
    return i1l < i2l;
}

NixDocMaker NixDocMaker::generate(const AppInfo& info, const ArgInfoVec& infos) {    
    NixDocMaker mkr;

    mkr.create_intro(info);
    mkr.create_usage(infos);
    mkr.create_arg_section(infos);
    mkr.create_end(info);

    return mkr;
}

void NixDocMaker::to_screen() const {
    std::cout << res << "\n";
}

void NixDocMaker::create_intro(const AppInfo& info) {
    res.append(util::cat(
        info.name, " - ", info.description.brief,
        "\n\n", info.description.full, "\n"
    ));
}

void NixDocMaker::create_usage(const ArgInfoVec& infos) {
    std::string usage = util::cat("\nusage:\n\t./app");

    if (has_options(infos))
        usage.append(" [options]");

    for (const auto& arg_info: infos) {
        if (arg_info.type != Type::positional)
            continue;
        
        std::string app{arg_info.value.value()};
        if (!arg_info.required)
            app = util::cat("[", app, "]");
        
        usage.append(util::cat(" ", app));
    }

    res.append(util::cat(usage, "\n"));
}

bool NixDocMaker::has_options(const ArgInfoVec& infos) {
    for (const auto& info: infos) {
        if (info.type != Type::positional)
            return true;
    }
    return false;
}

void NixDocMaker::create_arg_section(const ArgInfoVec& infos) {
    ArgInfoVec non_pos;
    std::copy_if(infos.begin(), infos.end(), std::back_inserter(non_pos), [](const ArgInfo& info) {
        return info.type != Type::positional;
    });
    res.append(util::cat(
        "\noptions:\n", arg_info_vec_to_str(non_pos)
    ));
}

void NixDocMaker::create_end(const AppInfo& info) {
    res.append(util::cat(
        info.description.epilog, "\n\nauthors:"
    ));

    for (const auto& author: info.authors) {
        res.append(util::cat(" ", author, ","));
    }

    res.append("\b \n");
}

std::string NixDocMaker::arg_info_vec_to_str(ArgInfoVec& info_vec) {
    std::sort(info_vec.begin(), info_vec.end(), arg_info_comp);
    
    std::string res{""};
    for (const ArgInfo& info: info_vec) 
        res += arg_info_to_str(info);
    return res;
}

std::string NixDocMaker::arg_info_to_str(const ArgInfo& info) {
    std::string res{"\t"};

    if (info.short_version.has_value())
        res += util::cat("-", info.short_version.value(), " ");
    if (info.long_version.has_value())
        res += util::cat("--", info.long_version.value(), " ");
    if (info.value.has_value())
        res += util::cat("<", util::upper(info.value.value()), "> ");
    
    res += util::cat("\b, ", info.description, "\n");
    return res;
}


}