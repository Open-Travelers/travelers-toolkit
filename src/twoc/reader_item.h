#ifndef READER_ITEM_H
#define READER_ITEM_H

#include "binary_reader.h"
#include <iostream>
#define __ITEM_DECLARE_MEMBER_I(type, name) type name;
#define __ITEM_DECLARE_MEMBER_A(type, name, count) type name[count];
#define __ITEM_COUNT_I(type, name) + sizeof(type)
#define __ITEM_COUNT_A(type, name, count) + (sizeof(type) * count)

#define __ITEM_DECLARE_GETTER_I(type, name) type name() const { return m_data.name; }
#define __ITEM_DECLARE_GETTER_A(type, name, count) type name(int index) const { return m_data.name[index]; }

//#define __ITEM_READ_MEMBER_I(type, name) std::cout << reader.tell() << ": "; m_data.name = reader.read<type>(); std::cout << #name << ": " << m_data.name << std::endl;
//#define __ITEM_READ_MEMBER_A(type, name, count) for (int i = 0; i < count; i++) { m_data.name[i] = reader.read<type>(); std::cout << #name << "[" << i << "]: " << m_data.name[i] << std::endl; }

#define __ITEM_READ_MEMBER_I(type, name) m_data.name = reader.read<type>();
#define __ITEM_READ_MEMBER_A(type, name, count) for (int i = 0; i < count; i++) { m_data.name[i] = reader.read<type>(); }

#define ITEM_DECLARE_EX(name, inherit, x) \
struct __attribute__((__packed__)) name##Data {\
    x(__ITEM_DECLARE_MEMBER_I, __ITEM_DECLARE_MEMBER_A) \
    name##Data() = default; \
}; \
class name : public inherit {\
protected:\
    name##Data m_data;\
public:\
    name() = default; \
    ~name() override; \
    name##Data const& data() const { return m_data; } \
    static name *from_reader(Twoc::BinaryReader &reader, size_t fullsize = 0); \
    size_t size() override; \
    bool validate() override; \
    bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) override; \
    x(__ITEM_DECLARE_GETTER_I, __ITEM_DECLARE_GETTER_A) \
};
#define ITEM_DECLARE(name, x) ITEM_DECLARE_EX(name, Twoc::IReaderItem, x)

#define ITEM_DEFINE(name, x) \
    size_t name::size() \
    {\
        return 0 x(__ITEM_COUNT_I, __ITEM_COUNT_A);\
    }\
    name *name::from_reader(Twoc::BinaryReader &reader, size_t fullsize) \
    {\
        name *result = new name();\
        if (!result) \
            return nullptr; \
        if (!result->read(reader, fullsize)) \
        {\
                delete result; \
                return nullptr; \
        }\
        return result; \
    }\
    bool name::read(Twoc::BinaryReader &reader, size_t fullsize) \
    {\
        x(__ITEM_READ_MEMBER_I, __ITEM_READ_MEMBER_A) \
        if ((fullsize != 0) && (size() < fullsize)) \
            reader.seek(Twoc::ReaderBase::Current, fullsize - size());\
        else if ((fullsize != 0) && (size() > fullsize)) \
            std::cerr << #name << ": BAD SIZE, EXPECTED " << size() << " WANTED " << fullsize << std::endl; \
        if (reader.status() == Twoc::ReaderStatus::Error) \
            return false; \
        if (!validate())\
            return false;\
        return true; \
    }\
    name::~name() \
    {\
    }

namespace Twoc
{

class IReaderItem {
public:
    virtual ~IReaderItem() = default;
    virtual bool read(Twoc::BinaryReader &reader, size_t fullsize = 0) = 0;
    virtual size_t size() = 0;
protected:
    virtual bool validate() = 0;
};

}

#endif // READER_ITEM_H
