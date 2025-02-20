#include "Config.hpp"

ConfigValue::ConfigValue() : type(INT), i(0) {}

ConfigValue::ConfigValue(int i) : type(INT), i(i) {}

ConfigValue::ConfigValue(bool b) : type(BOOL), b(b) {}

ConfigValue::ConfigValue(std::string const &str) : type(STRING)
{
    s = str;
}

ConfigValue::ConfigValue(config_array const &array) : type(ARRAY)
{
    a = array;
}

ConfigValue::ConfigValue(config_map const &map) : type(MAP)
{
    m = map;
}

ConfigValue::ConfigValue(ConfigValue const &other)
{
    type = other.type;
    switch (type)
    {
    case INT:
        i = other.i;
        break;
    case BOOL:
        b = other.b;
        break;
    case STRING:
        s = other.s;
        break;
    case ARRAY:
        a = other.a;
        break;
    case MAP:
        m = other.m;
        break;
    }
}

ConfigValue::~ConfigValue()
{
    clear();
}

void ConfigValue::clear()
{
    // if (type == STRING)
    //     delete s;
    // if (type == ARRAY)
    //     delete a;
    // if (type == MAP)
    //     delete m;
}

// Operators

ConfigValue &ConfigValue::operator=(const ConfigValue &other)
{
    if (this != &other)
    {
        clear(); // Free old memory
        type = other.type;
        switch (type)
        {
        case INT:
            i = other.i;
            break;
        case BOOL:
            b = other.b;
            break;
        case STRING:
            s = other.s;
            break;
        case ARRAY:
            a = other.a;
            break;
        case MAP:
            m = other.m;
            break;
        }
    }
    return *this;
}

ConfigValue::operator std::string() const
{
    return (type == STRING) ? s : "";
}

ConfigValue::operator int() const
{
    return (type == INT) ? i : 0;
}

ConfigValue::operator bool() const
{
    return (type == BOOL) ? b : false;
}

ConfigValue::operator config_array() const
{
    return (type == ARRAY) ? a : config_array();
}

ConfigValue::operator config_map() const
{
    return (type == MAP) ? m : config_map();
}

bool ConfigValue::operator==(const std::string &other) const
{
    return (type == STRING) && (s == other);
}

bool ConfigValue::operator==(const int &other) const
{
    return (type == INT) && (i == other);
}

bool ConfigValue::operator==(const bool &other) const
{
    return (type == BOOL) && (b == other);
}

std::ostream& operator<<(std::ostream& os, const ConfigValue& cv) {
    switch (cv.getType()) {
        case ConfigValue::INT:
            os << cv.getInt();
            break;
        case ConfigValue::BOOL:
            os << (cv.getBool() ? "true" : "false");
            break;
        case ConfigValue::STRING:
            os << '"' << cv.getString() << '"';
            break;
        case ConfigValue::ARRAY:
            os << "[ ";
            for (size_t i = 0; i < cv.getArray().size(); i++) {
                if (i > 0) os << ", ";
                os << (cv.getArray())[i];
            }
            os << " ]";
            break;
        case ConfigValue::MAP:
            os << "{ ";
            for (config_map::const_iterator it = cv.getMap().begin(); it != cv.getMap().end(); ++it) {
                if (it != cv.getMap().begin()) os << ", ";
                os << it->first << ": " << it->second;
            }
            os << " }";
            break;
    }
    return os;
}

// Member functions

ConfigValue ConfigValue::detectType(const std::string &value)
{
    if (value == "true")
        return ConfigValue(true);
    if (value == "false")
        return ConfigValue(false);

    bool isNumber = true;
    for (size_t i = 0; i < value.size(); i++)
    {
        if (!isdigit(value[i]))
        {
            isNumber = false;
            break;
        }
    }
    if (isNumber)
        return ConfigValue(atoi(value.c_str()));

    return ConfigValue(value);
}
