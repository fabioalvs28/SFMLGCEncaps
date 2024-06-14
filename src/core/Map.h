#pragma once
#include <assert.h>
#include <map>


template <typename Key, typename Value>

class GCMap
{
public:
	// Default constructor
	GCMap() : m_Map() {}

	// Constructor with initializer list
	GCMap(std::initializer_list<typename std::map<Key, Value>::value_type> init)
		: m_Map(init) {}

	// Copy constructor
	GCMap(const GCMap& other) : m_Map(other.m_Map) {}

	// Move constructor
	GCMap(GCMap&& other) noexcept : m_Map(std::move(other.m_Map)) {}

	// Assignment operators
	GCMap& operator=(const GCMap& other)
	{
		if (this != &other) // Check for self-assignment
			m_Map = other.m_Map;
		return *this;
	}

	GCMap& operator=(GCMap&& other) noexcept
	{
		m_Map = std::move(other.m_Map);
		return *this;
	}

	void Insert(const Key& key, const Value& value);
	void Remove(const Key& key);
	bool Find(const Key& key, Value& value);
	const Value& GetValue(const Key& key) const;
	void Clear();
	size_t GetSize() const;
	bool IsEmpty() const;

	Value& operator[](const Key& key) { return m_Map[key]; }
	const Value& operator[](const Key& key) const { return m_Map[key]; }

	// Iterator support
	typename std::map<Key, Value>::iterator begin() { return m_Map.begin(); }
	typename std::map<Key, Value>::iterator end() { return m_Map.end(); }
	typename std::map<Key, Value>::const_iterator begin() const { return m_Map.begin(); }
	typename std::map<Key, Value>::const_iterator end() const { return m_Map.end(); }

private:
	std::map<Key, Value> m_Map;
};


template<typename Key, typename Value>
void GCMap<Key, Value>::Insert(const Key& key, const Value& value)
{
	m_Map[key] = value;
}

template<typename Key, typename Value>
void GCMap<Key, Value>::Remove(const Key& key)
{
	m_Map.erase(key);
}

template<typename Key, typename Value>
bool GCMap<Key, Value>::Find(const Key& key, Value& value)
{
	auto it = m_Map.find(key);

	if (it != m_Map.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

template<typename Key, typename Value>
const Value& GCMap<Key, Value>::GetValue(const Key& key) const
{
	auto it = m_Map.find(key);
	assert(it != m_Map.end());
	return it->second;
}

template<typename Key, typename Value>
void GCMap<Key, Value>::Clear()
{
	m_Map.clear();
}

template<typename Key, typename Value>
size_t GCMap<Key, Value>::GetSize() const
{
	return m_Map.size();
}

template<typename Key, typename Value>
bool GCMap<Key, Value>::IsEmpty() const
{
	return m_Map.empty();
}