#include "pch.h"
#include "GCString.h"


GCString::GCString()
    : m_data(new char[1]), m_length(0)
{
    m_data[0] = '\0';
}

GCString::GCString(const char* str)
    : m_data(new char[strlen(str) + 1]), m_length(strlen(str))
{
    std::copy(str, str + m_length + 1, m_data);
}

GCString::GCString(const GCString& other) 
    : m_data(new char[other.m_length + 1]), m_length(other.m_length)
{
    std::copy(other.m_data, other.m_data + other.m_length + 1, m_data);
}

GCString::GCString(GCString&& other) noexcept
    : m_data(other.m_data), m_length(other.m_length)
{
    other.m_data = nullptr;
    other.m_length = 0;
}

GCString::~GCString()
{
    delete[] m_data;
}
