#include "core/pch.hh"

#include "core/cmdline.hh"
#include "core/logging.hh"

static std::unordered_set<std::string> s_options;
static std::unordered_map<std::string, std::string> s_arguments;

static inline bool isOptionString(const std::string& string)
{
    if(string.find_last_of(CMDLINE_OPTION_PREFIX) >= (string.size() - 1))
        return false;
    return string[0] == CMDLINE_OPTION_PREFIX;
}

static inline std::string getOption(const std::string& string)
{
    std::size_t i;
    for(i = 0; string[i] == CMDLINE_OPTION_PREFIX; ++i)
        ;
    return std::string(string.cbegin() + i, string.cend());
}

void cmdline::init(int argc, char** argv)
{
    for(int idx = 1; idx < argc; ++idx) {
        std::string option_argv(argv[idx]);

        if(!isOptionString(option_argv)) {
            logging::warn("cmdline: unknown option: {}", option_argv);
            continue;
        }

        auto option_string = getOption(option_argv);
        auto next_index = idx + 1;

        if(next_index < argc) {
            std::string argument_argv(argv[next_index]);

            if(!isOptionString(argument_argv)) {
                s_arguments.insert_or_assign(option_string, argument_argv);
                idx = next_index;
                continue;
            }
        }

        s_options.insert(option_string);
    }
}

void cmdline::add(std::string_view option)
{
    std::string key(option);
    if(s_arguments.count(key))
        s_arguments.erase(key);
    s_options.insert(key);
}

void cmdline::add(std::string_view option, std::string_view argument)
{
    std::string key(option);
    if(s_options.count(key))
        s_options.erase(key);
    s_arguments.insert_or_assign(key, argument);
}

bool cmdline::contains(std::string_view option)
{
    std::string key(option);
    return s_options.count(key) || s_arguments.count(key);
}

const char* cmdline::getArgument(std::string_view option, const char* fallback)
{
    auto argument = s_arguments.find(std::string(option));
    if(argument != s_arguments.cend())
        return argument->second.c_str();
    return fallback;
}
