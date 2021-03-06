#pragma once

#include <sstream>
#include <array>
#include <algorithm>
#include <cstring>
#include <type_traits>
#include <functional>


// This class is a statically allocated version of a memory continuous map, mainly useful for small data sets.
// Notice that this is a multimap! Inserting the same key twice will result with duplicate entries (sorted by order of insertion).
// Getters will always return the first matching entry
template <
    class _KeyType,
    class _ValueType,
    size_t _MaxMembers,
    class _Compare = std::less<_KeyType>
>
class StaticFlatMap
    : private _Compare
{
	static_assert(std::is_trivially_copyable<_KeyType>::value,
			"StaticFlatMap key type must be IsTriviallyCopyable");
	static_assert(std::is_trivially_copyable<_ValueType>::value,
			"StaticFlatMap value type must be IsTriviallyCopyable");
	static_assert(std::is_default_constructible<_ValueType>::value,
			"StaticFlatMap value type must be default constructible");

public:
	using KeyType = _KeyType;
	using ValueType = _ValueType;
	using KeyValuePair = std::pair<KeyType, ValueType>;
	using ContainerType = std::array<KeyValuePair, _MaxMembers>;

	// Iterators
	using iterator = typename ContainerType::iterator;
	using const_iterator = typename ContainerType::const_iterator;
	using reverse_iterator = typename ContainerType::reverse_iterator;
	using const_reverse_iterator = typename ContainerType::const_reverse_iterator;
	using value_type = KeyValuePair;
	using key_type = KeyType;
	using mapped_type = ValueType;
	using key_compare = _Compare;

	struct value_compare {
		_Compare comp;
		value_compare(_Compare c) : comp(c) {}

		bool operator()(const value_type& x, const value_type& y) const {
			return comp(x.first, y.first);
		}
	};

	StaticFlatMap(const std::initializer_list<KeyValuePair>& values)
	{
		for (auto& value : values)
		{
			Insert(value);
		}
	}

	constexpr StaticFlatMap() noexcept {}

	iterator Insert(const KeyValuePair& val)
	{
		auto position = std::upper_bound(begin(), end(), val, compareFunction);
		insertByIterator(position, val);
		return position;
	}

	ValueType& at(const KeyType& key)
	{
		auto elem = Find(key);
		if (elem == end())
		{
			throwOutOfRangeError(key, __PRETTY_FUNCTION__);
		}
		return elem->second;
	}

	iterator Find(const KeyType& key) noexcept
	{
		return const_cast<iterator>(static_cast<const StaticFlatMap&>(*this).Find(key));
	}

	const_iterator Find(const KeyType& key) const noexcept
	{
		KeyValuePair dummyPair{key, ValueType()};
		auto it = std::lower_bound(&m_sortedArray[0], &m_sortedArray[m_endIndex],
				dummyPair, compareFunction);
		return it != end() && !key_comp()(key, it->first) ? it : end();
	}

	iterator Erase(const_iterator position)
	{
		if (position == end() || m_endIndex == 0)
		{
			throwRangeError(*position, __PRETTY_FUNCTION__);
		}
		auto nonConstIter = (iterator)position;
		std::copy(nonConstIter + 1, end(), nonConstIter);
		--m_endIndex;
		return nonConstIter;
	}

	iterator Erase(const KeyType& key) { return Erase(Find(key)); }

	ValueType& operator[](const KeyType& key)
	{
		KeyValuePair dummyPair{key, ValueType()};
		auto range = std::equal_range(begin(), end(), dummyPair, compareFunction);
		if (range.first == range.second)
		{
			// value was not found, inserting it in the right location
			insertByIterator(range.first, dummyPair);
		}
		return range.first->second;
	}

	// std::map compatibility
	template <class... Params>
	iterator erase(const_iterator position)    { return Erase(position); }
	iterator erase(const KeyType& key)         { return Erase(key);      }
	iterator insert(const KeyValuePair& val)   { return Insert(val);     }
	iterator find(const KeyType& key) noexcept { return Find(key);       }
	const_iterator find(const KeyType& key) const noexcept { return Find(key); }

	void Clear() noexcept { m_endIndex = 0; }
	void clear() noexcept { Clear(); }

	iterator begin()                 noexcept { return iterator(&m_sortedArray[0]);                  }
	iterator end()                   noexcept { return iterator(&m_sortedArray[m_endIndex]);         }
	reverse_iterator rbegin()        noexcept { return reverse_iterator(&m_sortedArray[m_endIndex]); }
	reverse_iterator rend()          noexcept { return reverse_iterator(&m_sortedArray[0]);          }

	const_iterator cbegin()          const noexcept { return const_iterator(&m_sortedArray[0]);                  }
	const_iterator cend()            const noexcept { return const_iterator(&m_sortedArray[m_endIndex]);         }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(&m_sortedArray[m_endIndex]); }
	const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(&m_sortedArray[0]);          }

	const_iterator begin()          const noexcept { return const_iterator (&m_sortedArray[0]);                 }
	const_iterator end()            const noexcept { return const_iterator(&m_sortedArray[m_endIndex]);         }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(&m_sortedArray[m_endIndex]); }
	const_reverse_iterator rend()   const noexcept { return const_reverse_iterator(&m_sortedArray[0]);          }

	bool   empty()    const noexcept { return size() == 0; }
	size_t size()     const noexcept { return m_endIndex; }
	constexpr size_t max_size() const noexcept { return capacity(); }
	constexpr size_t capacity() const noexcept { return _MaxMembers; }
	key_compare key_comp() const noexcept { return *this; }
	value_compare value_comp() const noexcept { return value_compare{key_comp()}; }

private:

	void insertByIterator(const iterator& position, const KeyValuePair& val)
	{
		if (size() == _MaxMembers)
			throwRangeError(val, __PRETTY_FUNCTION__);
		std::copy_backward(position, end(), end() + 1);
		*position = val;
		++m_endIndex;
	}

	void throwRangeError(const KeyValuePair& val, const char* throwingFunction)
	{
		std::stringstream errorMessage;
		errorMessage << throwingFunction << " : Out of range! key = " << val.first << " value = " << val.second;
		throw std::range_error(errorMessage.str().c_str());
	}

	void throwOutOfRangeError(const KeyType& key, const char* throwingFunction)
	{
		std::stringstream errorMessage;
		errorMessage << throwingFunction << " : Could not find object in map! key = " << key;
		throw std::out_of_range(errorMessage.str().c_str());
	}

	static bool compareFunction(const KeyValuePair& first, const KeyValuePair& second) noexcept
	{
		static _Compare less;
		return less(first.first, second.first);
	}

	ContainerType m_sortedArray;
	size_t        m_endIndex = 0;
};

