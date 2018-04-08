#pragma once

#include "../DataStruct/HashTable.h"
#include <assert.h>
#include <string>

namespace GameSystem {

	class Property
	{
	protected:
		union Data
		{
			int* m_pInt;
			float* m_pFloat;
			std::string* m_pString;
			bool* m_pBool;
		};

		enum Type
		{
			EMPTY,
			INT,
			FLOAT,
			STRING,
			BOOL
		};

	protected:
		Data m_data;
		Type m_type;
		std::string m_name;

	private:
		Property& Copy(const Property& obj);
		Property& operator= (const Property& obj);

	protected:
		void EraseType();
		void Register(int* value);
		void Register(float* valute);
		void Register(std::string* new_string);
		void Register(bool* valute);

	public:
		Property();
		Property(const Property& source);
		Property(std::string const& name);
		Property(std::string const& name, int* value);
		Property(std::string const& name, float* value);
		Property(std::string const& name, std::string* value);
		Property(std::string const& name, bool* value);

	public:
		bool SetUnknownValue(std::string const& value);
		bool Set(int value);
		bool Set(float value);
		bool Set(std::string const& value);
		bool Set(bool value);
		void SetName(std::string const& name);
		std::string GetName() const;
		int GetInt();
		float GetFloat();
		std::string GetString();
		bool GetBool();

		std::string GetValue();
	};

	class PropertySet
	{
	private:
		HashTable<Property> m_properties;
	
	private:
		PropertySet& Copy(const PropertySet& src_obj);
		PropertySet& operator= (const PropertySet& src_obj);

	public:
		PropertySet();
		PropertySet(const PropertySet& src) { Copy(src); }
		virtual ~PropertySet();

		void Register(std::string const& name, int* pValue);
		void Register(std::string const& name, float* pValue);
		void Register(std::string const& name, std::string* pValue);
		void Register(std::string const& name, bool* pValue);

		Property* Lookup(std::string const& name);
		bool SetValue(std::string const& name, std::string const& value);
		bool Set(std::string const& name, std::string const& value);
		bool Set(std::string const& name, int value);
		bool Set(std::string const& name, float value);
		bool Set(std::string const& name, bool value);
		bool Set(std::string const& name, char* pValue);
	};
}