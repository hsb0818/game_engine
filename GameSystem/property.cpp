#include "property.h"

namespace GameSystem {

	Property& Property::Copy(const Property& obj)
	{
		m_data = obj.m_data;
		m_type = obj.m_type;
		m_name = obj.m_name;

		return *this;
	}

	Property& Property::operator= (const Property& obj)
	{
		return Copy(obj);
	}

	Property::Property()
	{
		EraseType();
	}

	Property::Property(const Property& source)
	{
		Copy(source);
	}

	Property::Property(std::string const& name)
	{
		EraseType();
		m_name = name;
	}

	Property::Property(std::string const& name, int* pValue)
	{
		m_name = name;
		Register(pValue);
	}

	Property::Property(std::string const& name, float* pValue)
	{
		m_name = name;
		Register(pValue);
	}

	Property::Property(std::string const& name, std::string* pValue)
	{
		m_name = name;
		Register(pValue);
	}

	Property::Property(std::string const& name, bool* pValue)
	{
		m_name = name;
		Register(pValue);
	}

	void Property::EraseType()
	{
		m_type = EMPTY;
		m_data.m_pInt = 0; // possible to initialize like this one.
		m_name = "";
	}

	void Property::Register(int* pValue)
	{
		m_type = INT;
		m_data.m_pInt = pValue;
	}

	void Property::Register(float* pValue)
	{
		m_type = FLOAT;
		m_data.m_pFloat = pValue;
	}

	void Property::Register(std::string* new_string)
	{
		m_type = Type::STRING;
		m_data.m_pString = new_string;
	}

	void Property::Register(bool* pValue)
	{
		m_type = Type::BOOL;
		m_data.m_pBool = pValue;
	}

	bool Property::SetUnknownValue(std::string const& value)
	{
		bool return_value = true;
		switch(m_type)
		{
		case Type::INT:
			*m_data.m_pInt = atoi(value.c_str());
			break;
		case Type::FLOAT:
			*m_data.m_pFloat = (float)atof(value.c_str());
			break;
		case Type::STRING:
			*m_data.m_pString = value;
			break;
		case Type::BOOL:
			*m_data.m_pBool = (value.compare("true") == 0) || (value.compare("TRUE") == 0);
			break;
		default:
			return_value = false;
			break;
		}

		return return_value;
	}

	bool Property::Set(int value)
	{
		bool return_value = false;
		if (m_type == Type::INT)
		{
			*m_data.m_pInt = value;
			return_value = true;
		}

		return return_value;
	}

	bool Property::Set(float value)
	{
		bool return_value = false;
		if(m_type == Type::FLOAT)
		{
			*m_data.m_pFloat = value;
			return_value = true;
		}

		return return_value;
	}

	bool Property::Set(std::string const& value)
	{
		bool return_value = false;
		if(m_type == Type::STRING)
		{
			*m_data.m_pString = value;
			return_value = true;
		}

		return return_value;
	}

	bool Property::Set(bool value)
	{
		bool return_value = false;
		if(m_type == Type::BOOL)
		{
			*m_data.m_pBool = value;
			return_value = true;
		}

		return return_value;
	}

	void Property::SetName(std::string const& name)
	{
		m_name = name;
	}

	std::string Property::GetName() const
	{
		return m_name;
	}

	int Property::GetInt()
	{
		assert(m_type == INT);
		return *m_data.m_pInt;
	}

	float Property::GetFloat()
	{
		assert(m_type == FLOAT);
		return *m_data.m_pFloat;
	}

	std::string Property::GetString()
	{
		assert(m_type == STRING);
		return *m_data.m_pString;
	}

	bool Property::GetBool()
	{
		assert(m_type == BOOL);
		return *m_data.m_pBool;
	}

	std::string Property::GetValue()
	{
		using namespace std;
		string return_value;

		char buffer[64];

		switch(m_type)
		{
		case Type::INT:
			sprintf_s(buffer, "%d", *m_data.m_pInt);
			return_value = buffer;
			break;
		case Type::FLOAT:
			sprintf_s(buffer, "%f", *m_data.m_pFloat);
			return_value = buffer;
			break;
		case Type::STRING:
			return_value = *m_data.m_pString;
			break;
		case Type::BOOL:
			if (*m_data.m_pBool)
				sprintf_s(buffer, "true");
			else
				sprintf_s(buffer, "false");

			return_value = buffer;
			break;
		default:
			break;
		}

		return return_value;
	}

	// PropertySet
	PropertySet::PropertySet()
	{
	}

	PropertySet::~PropertySet()
	{
	}

	void PropertySet::Register(std::string const& name, int* pValue)
	{
		Property* new_property = new Property(name, pValue);
		m_properties.Insert(name, new_property);
	}

	void PropertySet::Register(std::string const& name, float* pValue)
	{
		Property* new_property = new Property(name, pValue);
		m_properties.Insert(name, new_property);
	}
	void PropertySet::Register(std::string const& name, std::string* pValue)
	{
		Property* new_property = new Property(name, pValue);
		m_properties.Insert(name, new_property);
	}

	void PropertySet::Register(std::string const& name, bool* pValue)
	{
		Property* new_property = new Property(name, pValue);
		m_properties.Insert(name, new_property);
	}

	Property* PropertySet::Lookup(std::string const& name) 
	{
		return m_properties.Lookup(name); 
	}

	bool PropertySet::SetValue(std::string const& name, std::string const& value)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);

		if (ppt)
		{
			return_value = ppt->SetUnknownValue(value);
		}

		return return_value;
	}

	bool PropertySet::Set(std::string const& name, std::string const& value)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);
		if(ppt)
		{
			return_value = ppt->Set(value);
		}

		return return_value;
	}

	bool PropertySet::Set(std::string const& name, int value)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);
		if(ppt)
		{
			return_value = ppt->Set(value);
		}

		return return_value;
	}

	bool PropertySet::Set(std::string const& name, float value)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);
		if(ppt)
		{
			return_value = ppt->Set(value);
		}

		return return_value;
	}

	bool PropertySet::Set(std::string const& name, bool value)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);
		if(ppt)
		{
			return_value = ppt->Set(value);
		}

		return return_value;
	}

	bool PropertySet::Set(std::string const& name, char* pValue)
	{
		bool return_value = false;
		Property* ppt = Lookup(name);
		if(ppt)
		{
			return_value = ppt->Set(std::string(pValue));
		}

		return return_value;
	}
}