#include "core/pch.hh"

#include "core/cmdline.hh"

#include "core/config.hh"
#include "core/cvar.hh"
#include "core/strtools.hh"
#include "core/version.hh"

Config::Config(std::string_view filename)
{
    loadFile(filename);
}

bool Config::loadFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::in);

    if(!file.is_open()) {
        spdlog::warn("config: cannot open {}", path.string());

        return false;
    }

    std::string line;
    std::string kv_string;

    while(std::getline(file, line)) {
        auto comment = line.find_first_of('#');

        if(comment == std::string::npos) {
            kv_string = strtools::trimWhitespace(line);
        }
        else {
            kv_string = strtools::trimWhitespace(line.substr(0, comment));
        }

        if(strtools::isWhitespace(kv_string)) {
            // Ignore empty or commented out lines
            continue;
        }

        auto separator = kv_string.find_first_of('=');

        if(separator == std::string::npos) {
            spdlog::warn("config: {}: invalid line: {}", path.string(), line);

            continue;
        }

        auto kv_name = strtools::trimWhitespace(kv_string.substr(0, separator));
        auto kv_value = strtools::trimWhitespace(kv_string.substr(separator + 1));

        if(auto cvar = find(kv_name)) {
            cvar->rawSetValue(kv_value.c_str());
            continue;
        }
    }

    return true;
}

bool Config::saveFile(const std::filesystem::path& path) const
{
    std::ofstream file(path, std::ios::out);

    if(!file.is_open()) {
        spdlog::warn("config: cannot open {} for writing", path.string());

        return false;
    }

    auto curtime = std::time(nullptr);

    file << "# QFortress " << PROJECT_VERSION_STRING << " configuration file" << std::endl;
    file << "# Generated at: " << std::put_time(std::gmtime(&curtime), "%Y-%m-%d %H:%M:%S %z") << std::endl << std::endl;

    for(const auto cvar : m_cvar_map) {
        file << cvar.first << "=";
        file << cvar.second->rawGetValue();
        file << std::endl;
    }

    return true;
}

void Config::parseCommandline(void)
{
    for(auto cvar : m_cvar_map) {
        if(auto argument = cmdline::getArgument(std::format("cvar:{}", cvar.first))) {
            cvar.second->rawSetValue(argument);
        }
    }
}

ICVar* Config::find(std::string_view name) const
{
    auto cvar = m_cvar_map.find(std::string(name));

    if(cvar == m_cvar_map.cend()) {
        return nullptr;
    }

    return cvar->second;
}

const char* Config::getValue(std::string_view name) const
{
    if(auto cvar = find(name)) {
        return cvar->rawGetValue();
    }

    return nullptr;
}

void Config::setValue(std::string_view name, const char* value)
{
    assert(value);

    if(auto cvar = find(name)) {
        cvar->rawSetValue(value);
    }
}
