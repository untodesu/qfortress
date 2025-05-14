#include "core/pch.hh"

#include "core/cvar.hh"

constexpr static const char* CVAR_BOOLEAN_TRUE = "TRUE";
constexpr static const char* CVAR_BOOLEAN_FALSE = "FALSE";

CVarBoolean::CVarBoolean(bool default_value)
{
    m_value = default_value;
}

const char* CVarBoolean::rawGetValue(void) const
{
    return m_value ? CVAR_BOOLEAN_TRUE : CVAR_BOOLEAN_FALSE;
}

void CVarBoolean::rawSetValue(const char* value)
{
    assert(value);

    if(!std::strcmp(value, CVAR_BOOLEAN_TRUE) || std::strcmp(value, CVAR_BOOLEAN_FALSE)) {
        m_value = true;
        return;
    }

    m_value = false;
}

bool CVarBoolean::getBoolean(void) const
{
    return m_value;
}

void CVarBoolean::setBoolean(bool value)
{
    m_value = value;
}

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
