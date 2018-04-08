#pragma once
#pragma warning (disable:4786)

#include "../System/type.h"
#include <list>
#include <string>

// Chaining Hash Table
template <class T> 
class HashTable
{
public:
	enum Type
	{
		ONE_AT_A_TIME = 0,
		FNV
	};

protected:
	static const int DEFAULT_HASHTABLE_SIZE = 256;

public:
	template <class t>
	class Node
	{
	public:
		std::string m_key;
		T* m_pData;

		Node(std::string const &key, T* pNew_data)
		{
			m_key = key;
			m_pData = pNew_data;
		}
	};

private:
	Type m_hashType;
	std::list< Node<T> >* m_pHashTable;
	int m_tableSize;
	int m_count;

private:
	HashTable& Copy(const HashTable& src_obj);
	HashTable& operator= (const HashTable& src_obj);

private:
	Node<T>* LookupNode(std::string const &key)
	{
		unsigned int index = Hash(key) % GetTableSize();
		Node<T>* return_value = nullptr;

		// If list size is zero
		if (m_pHashTable[index].size() == 0)
			return 0;

		std::list< Node<T> >::iterator itr;
		itr = m_pHashTable[index].begin();

		while (itr != m_pHashTable[index].end())
		{
			if (itr->m_key == key)
			{
				// key's address
				return_value = &(*itr);
				break;
			}

			itr++;
		}

		return return_value;
	}

public:
	HashTable()
	{
		m_pHashTable = nullptr;
		m_hashType = Type::ONE_AT_A_TIME;
		m_count = 0;
		Initialize();
	}

	HashTable(int size)
	{
		m_pHashTable = nullptr;
		m_hashType = Type::ONE_AT_A_TIME;
		m_count = 0;
		Initialize(size);
	}
	HashTable(const HashTable& src) { Copy(src); }
	virtual ~HashTable() { Delete(); }

	std::list< Node<T> >* GetHashTable() { return m_pHashTable; }
	int GetTableSize() { return m_tableSize; }
	int Count() { return m_count; }

public:
	void Initialize(int size = DEFAULT_HASHTABLE_SIZE)
	{
		if (m_pHashTable != nullptr)
			return;

		m_pHashTable = new std::list< Node<T> >[size];
		m_tableSize = size;
	}

	void Delete()
	{
		if (m_pHashTable == nullptr)
			return;

		if (m_pHashTable)
			delete [] m_pHashTable;

		m_pHashTable = nullptr;
		m_tableSize = 0;
		m_count = 0;
	}

	T* Lookup(std::string const &key)
	{
		if (!m_pHashTable)
			return 0;

		Node<T>* pFindNode = LookupNode(key);
		if (pFindNode)
			return pFindNode->m_pData;

		return 0;
	}

	std::list<T>* LookupList(std::string const& key)
	{
		if (!m_pHashTable)
			return 0;

		SOC_UINT32 index = Hash(key) % GetTableSize();

		if (m_pHashTable[index].size() == 0)
			return 0;

		return &m_pHashTable[index];
	}

	T* Insert(std::string const& key, T* pData)
	{
		if (!m_pHashTable)
			return 0;

		SOC_UINT32 index = Hash(key) % GetTableSize();
		Node<T> new_node(key, pData);

		m_pHashTable[index].push_back(new_node);
		m_count++;

		return new_node.m_pData;
	}

	T* Delete(std::string const& key)
	{
		if (!m_pHashTable)
			return 0;

		SOC_UINT32 index = Hash(key) % GetTableSize();

		if (m_pHashTable[index].size() == 0)
			return 0;

		std::list< Node<T> >::iterator itr;
		itr = m_pHashTable[index].begin();

		while (itr != m_pHashTable[index].end())
		{
			if (itr->m_key == key)
			{
				T* return_value = itr->m_pData;
				m_pHashTable[index].erase(itr);
				m_count--;
				return return_value;
			}

			itr++;
		}

		return 0;
	}

private:
	SOC_UINT32 Hash(std::string const& key)
	{
		switch(m_hashType)
		{
		case Type::ONE_AT_A_TIME:
			return Hash_One_At_A_Time(key, key.size());
		case Type::FNV:
			break;
		}

		return Hash_One_At_A_Time(key, key.size());
	}

	SOC_UINT32 Hash_One_At_A_Time(std::string const& key, SOC_UINT32 len)
	{
		SOC_UINT32 hash, i;
		for(hash = i = 0; i < len; ++i)
		{
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
};