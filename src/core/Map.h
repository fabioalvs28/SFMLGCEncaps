#pragma once
#include <assert.h>
#include <map>


template <typename Key, typename Value>

class GCMap
{
public:
	GCMap() = default;

	void Insert(const Key& key, const Value& value);
	void Remove(const Key& key);
	bool Find(const Key& key, Value& value);
	const Value& GetValue(const Key& key) const;
	void Clear();
	size_t GetSize() const;
	bool IsEmpty() const;

private:
	std::map<Key, Value> m_Map;
};


/// <summary>
/// Insert a new value into the map with the given key
/// </summary>
/// <typeparam name="Key">The key to access to the value</typeparam>
/// <typeparam name="Value">The value you want to add</typeparam>

template<typename Key, typename Value>
void GCMap<Key, Value>::Insert(const Key& key, const Value& value)
{
	m_Map[key] = value;
}

/// <summary>
/// Remove the value from the map with the given key
/// </summary>
/// <typeparam name="Key">The key to access to the value</typeparam>

template<typename Key, typename Value>
void GCMap<Key, Value>::Remove(const Key& key)
{
	m_Map.erase(key);
}

/// <summary>
/// Find the value from the map with the given key
/// </summary>
/// <typeparam name="Key">The key to access to the value</typeparam>
/// <typeparam name="Value">The value you want to find</typeparam>

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

/// <summary>
/// Get the value from the map with the given key
/// </summary>
/// <typeparam name="Key">The key to access to the value</typeparam>

template<typename Key, typename Value>
const Value& GCMap<Key, Value>::GetValue(const Key& key) const
{
	auto it = m_Map.find(key);
	assert(it != m_Map.end());
	return it->second;
}

/// <summary>
/// Clear the map
/// </summary>

template<typename Key, typename Value>
void GCMap<Key, Value>::Clear()
{
	m_Map.clear();
}

/// <summary>
/// Get the size of the map
/// </summary>

template<typename Key, typename Value>
size_t GCMap<Key, Value>::GetSize() const
{
	return m_Map.size();
}

/// <summary>
/// Check if the map is empty
/// </summary>
/// <returns>True if the map is empty, false otherwise</returns>

template<typename Key, typename Value>
bool GCMap<Key, Value>::IsEmpty() const
{
	return m_Map.empty();
}