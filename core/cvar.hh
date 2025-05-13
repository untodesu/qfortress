#ifndef CORE_CVAR_HH
#define CORE_CVAR_HH
#pragma once

class ICVar {
public:
    virtual ~ICVar(void) = default;
    virtual const char* rawGetValue(void) const = 0;
    virtual void rawSetValue(const char* value) = 0;
};

class CVarString final : public ICVar {
public:
    explicit CVarString(std::string_view default_value);
    virtual ~CVarString(void) override = default;

    virtual const char* rawGetValue(void) const override;
    virtual void rawSetValue(const char* value) override;

    virtual const std::string& getString(void) const;
    virtual void setString(const std::string& value);
    virtual void setString(std::string_view value);

private:
    std::string m_string;
};

template<typename T> concept CVarNumericType = std::is_arithmetic_v<T>;

template<CVarNumericType T>
class CVarNumeric final : public ICVar {
public:
    inline explicit CVarNumeric(T default_value);
    inline explicit CVarNumeric(T default_value, T min_value, T max_value);
    virtual ~CVarNumeric(void) override = default;

    virtual inline const char* rawGetValue(void) const override;
    virtual inline void rawSetValue(const char* value) override;

    inline T getNumber(void) const;
    inline T getMinValue(void) const;
    inline T getMaxValue(void) const;
    inline void setNumber(T value);

private:
    T m_value;
    T m_min_value;
    T m_max_value;
    std::string m_string;
};

using CVarFloat = CVarNumeric<float>;
using CVarInt = CVarNumeric<int>;
using CVarUint = CVarNumeric<unsigned>;
using CVarUsize = CVarNumeric<std::size_t>;

// ============================================================================
// Inline methods implementations go after this comment
// ============================================================================

template<CVarNumericType T>
inline CVarNumeric<T>::CVarNumeric(T default_value)
{
    if constexpr(std::is_floating_point_v<T>) {
        m_min_value = -std::numeric_limits<T>::max(); // Floating point always fucks things up I guess
        m_max_value = +std::numeric_limits<T>::max();
    }
    else {
        m_min_value = std::numeric_limits<T>::min();
        m_max_value = std::numeric_limits<T>::max();
    }

    setNumber(default_value);
}

template<CVarNumericType T>
inline CVarNumeric<T>::CVarNumeric(T default_value, T min_value, T max_value)
{
    m_min_value = min_value;
    m_max_value = max_value;
    setNumber(default_value);
}

template<CVarNumericType T>
inline const char* CVarNumeric<T>::rawGetValue(void) const
{
    return m_string.c_str();
}

template<CVarNumericType T>
inline void CVarNumeric<T>::rawSetValue(const char* value)
{
    std::istringstream istream(value);
    istream >> m_value;
    setNumber(m_value);
}

template<CVarNumericType T>
inline T CVarNumeric<T>::getNumber(void) const
{
    return m_value;
}

template<CVarNumericType T>
inline T CVarNumeric<T>::getMinValue(void) const
{
    return m_min_value;
}

template<CVarNumericType T>
inline T CVarNumeric<T>::getMaxValue(void) const
{
    return m_max_value;
}

template<CVarNumericType T>
inline void CVarNumeric<T>::setNumber(T value)
{
    m_value = std::clamp<T>(value, m_min_value, m_max_value);
    m_string = std::to_string(m_value);
}

#endif // CORE_CVAR_HH
