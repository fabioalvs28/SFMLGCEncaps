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
	Value& GetValue(const Key& key) const;
	void Clear();
	size_t GetSize() const;
	bool IsEmpty() const;

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
Value& GCMap<Key, Value>::GetValue(const Key& key) const
{
	auto it = m_Map.find(key);
	assert(it != m_Map.end());
	value = it->second;
	return value;
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