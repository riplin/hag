//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/regs.h>
#include <hag/ialloc.h>

namespace Hag { namespace Testing { namespace Mock {

class CustomPortHandler
{
public:
    inline CustomPortHandler(const char* name)
        : m_Name(name)
        , m_Next(NULL)
    {

    }

    virtual ~CustomPortHandler();

    virtual bool CanHandle(uint16_t port) = 0;
    virtual bool HasHandled(uint16_t port) = 0;
    virtual uint8_t Read8(uint16_t port) = 0;
    virtual uint16_t Read16(uint16_t port) = 0;
    virtual void Write8(uint16_t port, uint8_t value) = 0;
    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi) = 0;
    virtual void Write16(uint16_t port, uint16_t value) = 0;
    virtual void Report(CustomPortHandler* instance1) = 0;
    virtual bool HasDifferences(CustomPortHandler* instance1) = 0;
    virtual void Reset() = 0;
    virtual void Snapshot() = 0;
    virtual void Rollback() = 0;

    inline const char* GetName() { return m_Name; }
    inline CustomPortHandler* GetNext() { return m_Next; }
    inline void SetNext(CustomPortHandler* next) { m_Next = next; }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id);
    static const uint32_t s_ID;

private:
    const char* m_Name;
    CustomPortHandler* m_Next;
};

template<typename T> T* customporthandler_cast(CustomPortHandler* ptr)
{
    return (T*)(ptr != NULL ? ptr->CheckTypeId(T::s_ID) : NULL);
}

struct PortAndValue
{
    uint16_t Port;
    uint8_t Value;
};

void Initialize(IAllocator& allocator, PortAndValue* defaultPortsAndValues, uint16_t defaultPortsAndValuesCount, uint8_t* attributeControllerRegisters, uint8_t* ramdacRegisters);
void SelectInstance(int instance);

void Snapshot();
void Rollback();

void AddReadOnlyPort(const char* name, uint16_t port);
void AddIndexedPort(const char* name, uint16_t indexPort, uint8_t indexMask, uint16_t dataPort, uint16_t regCount, uint8_t* defaultValues);
void AddReadOnlyIndexedRegister(uint16_t port, uint8_t reg);
void SetDefaultMemory(uint8_t* memory, uint32_t offset, uint32_t size);

typedef void (*IndexedRegisterCheckCallback_t)(uint16_t port, uint16_t index, uint8_t modifiedValue, uint8_t originalValue, void* context);
typedef void (*RegisterCheckCallback_t)(uint16_t port, uint8_t modifiedValue, uint8_t originalValue, void* context);
typedef void (*BDAFieldCallback_t)(uint8_t field, uint8_t modifiedValue, uint8_t originalValue, void* context);

struct PortAndIndexAndValue
{
    uint16_t Port;
    uint16_t Index;
    uint8_t Value;
};

struct PortAndIndex
{
    uint16_t Port;
    uint16_t Index;
};

int VerifyPortsAndValues(int instance, PortAndValue* modifiedPortsAndValues, int modifiedCount,
                         uint16_t* readPorts, int readCount,
                         PortAndIndexAndValue* modifiedIndexedPortsAndValues, int modifiedIndexedCount,
                         PortAndIndex* readIndexedPorts, int readIndexedCount,
                         uint16_t* ignorePorts = NULL, int ignorePortsCount = 0,
                         PortAndIndex* ignoreIndexedPorts = NULL, int ignoreIndexedCount = NULL);

struct BDAFieldsAndValues
{
    uint8_t Field;
    uint8_t Value;
};

int VerifyBDAFieldsAndValues(int instance, BDAFieldsAndValues* modifiedFieldsAndValues, int modifiedCount,
                             uint8_t* readFields, int readCount,
                             uint8_t* ignoreFields = NULL, int ignoreCount = 0);

void FetchModifiedIndexedRegisters(int instance, IndexedRegisterCheckCallback_t callback, void* context);
void FetchModifiedRegisters(int instance, RegisterCheckCallback_t callback, void* context);
void FetchModifiedBDAFields(int instance, BDAFieldCallback_t callback, void* context);

void Report();
bool HasDifferences();
void Reset();

int Shutdown();

namespace Port
{
    uint8_t Read8(uint16_t port);
    uint16_t Read16(uint16_t port);
    void Write8(uint16_t port, uint8_t value);
    void Write16(uint16_t port, uint16_t value);
    void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi);
}

namespace Memory
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count = 1);

    template<typename Type, uint16_t offset> Type& RefAs(uint16_t count = 1)
    {
        return (Type&)Ref(offset, sizeof(Type), count);
    }

}

namespace BDA
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count = 1);

    template<typename Type, uint16_t offset> Type& RefAs(uint16_t count = 1)
    {
        return (Type&)Ref(offset, sizeof(Type), count);
    }
}

}}}

#define VERIFYPORTCONTENT(Instance, ModifiedPorts, ReadPorts, ModifiedIndexedPorts, ReadIndexedPorts)                               \
Hag::Testing::Mock::VerifyPortsAndValues(Instance, ModifiedPorts, sizeof(ModifiedPorts) / sizeof(Hag::Testing::Mock::PortAndValue), \
                    ReadPorts, sizeof(ReadPorts) / sizeof(uint16_t),                                                                \
                    ModifiedIndexedPorts, sizeof(ModifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue),          \
                    ReadIndexedPorts, sizeof(ReadIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndex));

#define VERIFYBDACONTENT(Instance, ModifiedFields, ReadFields)                                  \
Hag::Testing::Mock::VerifyBDAFieldsAndValues(Instance, ModifiedFields,                          \
                    sizeof(ModifiedFields) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),    \
                    ReadFields, sizeof(ReadFields) / sizeof(uint8_t));

#define VERIFYBDACONTENTIGNORE(Instance, ModifiedFields, ReadFields, IgnoreFields)              \
Hag::Testing::Mock::VerifyBDAFieldsAndValues(Instance, ModifiedFields,                          \
                    sizeof(ModifiedFields) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),    \
                    ReadFields, sizeof(ReadFields) / sizeof(uint8_t)                            \
                    IgnoreFields, sizeof(IgnoreFields) / sizeof(uint8_t));
