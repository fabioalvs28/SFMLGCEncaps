#pragma once

class GCString
{
public:
    /// <summary>
    /// Default constructor
    /// </summary>
    GCString();
    /// <summary>
    /// Constructor from C-style string
    /// </summary>
    /// <param name="str"></param>
    GCString(const char* str);
    /// <summary>
    /// Copy constructor
    /// </summary>
    /// <param name="other"></param>
    GCString(const GCString& other);
    /// <summary>
    /// Move constructor
    /// </summary>
    /// <param name="other"></param>
    GCString(GCString&& other) noexcept;
    /// <summary>
    /// Destructor
    /// </summary>
    ~GCString();

#pragma region operators
    /// <summary>
    /// Index operator
    /// </summary>
    char& operator[](size_t index) { return m_data[index]; }
    const char& operator[](size_t index) const { m_data[index]; }
    /// <summary>
    /// Equality operator
    /// </summary>
    bool operator==(const GCString& other) const
    {
        if (m_length != other.m_length) return false;
        return std::strcmp(m_data, other.m_data) == 0;
    }
    /// <summary>
    /// Inequality operator
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    bool operator!=(const GCString& other) const
    {
        return !(*this == other);
    }
    /// <summary>
    /// Copy assignment operator
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    GCString& operator=(const GCString& other)
    {
        if (this == &other) return *this;

        delete[] m_data;
        m_length = other.m_length;
        m_data = new char[m_length + 1];
        std::copy(other.m_data, other.m_data + m_length + 1, m_data);

        return *this;
    }
    /// <summary>
    /// Move assignment operator
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    GCString& operator=(GCString&& other) noexcept
    {
        if (this == &other) return *this;

        delete[] m_data;
        m_data = other.m_data;
        m_length = other.m_length;

        other.m_data = nullptr;
        other.m_length = 0;

        return *this;
    }
    /// <summary>
    /// Concatenates two GCStrings
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    GCString operator+(const GCString& other) const
    {
        GCString newStr;
        newStr.m_length = m_length + other.m_length;
        newStr.m_data = new char[newStr.m_length + 1];

        std::copy(m_data, m_data + m_length, newStr.m_data);
        std::copy(other.m_data, other.m_data + other.m_length + 1, newStr.m_data + m_length);

        return newStr;
    }
    /// <summary>
    /// Concatenates two GCStrings
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    GCString& operator+=(const GCString& other)
    {
        *this = *this + other;
        return *this;
    }
    /// <summary>
    /// Allows to print GCString to std::ostream using << operator
    /// </summary>
    /// <param name="os"></param>
    /// <param name="str"></param>
    /// <returns></returns>
    friend std::ostream& operator<<(std::ostream& os, const GCString& str)
    {
        os << str.m_data;
        return os;
    }
    /// <summary>
    /// Allows to read GCString from std::istream using >> operator
    /// </summary>
    /// <param name="is"></param>
    /// <param name="str"></param>
    /// <returns></returns>
    friend std::istream& operator>>(std::istream& is, GCString& str)
    {
        std::string temp;
        is >> temp;
        str = GCString(temp.c_str());
        return is;
    }
#pragma endregion

#pragma region iterators
    char* begin() { return m_data; }
    char* end() { return m_data + m_length; }
    const char* begin() const { return m_data; }
    const char* end() const { return m_data + m_length; }
#pragma endregion


#pragma region utilities

    size_t Length() const { return m_length; }
    const char* Data() const { return m_data; }
    
    bool Empty() const { return m_length == 0; }
    void Clear() { delete[] m_data; m_data = new char[1]; m_data[0] = '\0'; m_length = 0; }

    GCString Substring(size_t start, size_t length) const
    {
        GCString newStr;
        newStr.m_length = length;
        newStr.m_data = new char[length + 1];
        std::copy(m_data + start, m_data + start + length, newStr.m_data);
        newStr.m_data[length] = '\0';
        return newStr;
    }

    size_t Find(char c) const
    {
        const char* found = std::strchr(m_data, c);
        if (found == nullptr) return -1;
        return found - m_data;
    }

    size_t Find(const GCString& str) const
    {
        const char* found = std::strstr(m_data, str.m_data);
        if (found == nullptr) return -1;
        return found - m_data;
    }

    void Append(const char* str)
    {
        size_t newLength = m_length + std::strlen(str);
        char* newData = new char[newLength + 1];
        std::copy(m_data, m_data + m_length, newData);
        std::copy(str, str + std::strlen(str) + 1, newData + m_length);
        delete[] m_data;
        m_data = newData;
        m_length = newLength;
    }
#pragma endregion

private:
    char* m_data;
    size_t m_length;
};
