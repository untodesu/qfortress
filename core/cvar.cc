#include "core/pch.hh"

#include "core/cvar.hh"

CVarString::CVarString(std::string_view default_value)
{
    m_string = default_value;
}

const char* CVarString::rawGetValue(void) const
{
    return m_string.c_str();
}

void CVarString::rawSetValue(const char* value)
{
    assert(value);

    m_string = value;
}

const std::string& CVarString::getString(void) const
{
    return m_string;
}

void CVarString::setString(const std::string& value)
{
    m_string = value;
}

void CVarString::setString(std::string_view value)
{
    m_string = value;
}
