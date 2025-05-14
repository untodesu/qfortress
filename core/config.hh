#ifndef CORE_CONFIG_HH
#define CORE_CONFIG_HH
#pragma once

class ICVar;

class Config final {
public:
    explicit Config(void) = default;
    explicit Config(std::string_view filename);
    virtual ~Config(void) = default;

    bool loadFile(const std::filesystem::path& path);
    bool saveFile(const std::filesystem::path& path) const;

    void parseCommandline(void);

    ICVar* find(std::string_view name) const;

    const char* getValue(std::string_view name) const;
    void setValue(std::string_view name, const char* value);

private:
    std::unordered_map<std::string, ICVar*> m_cvar_map;
};

#endif // CORE_CONFIG_HH
