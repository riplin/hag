//Copyright 2023-Present riplin

#include <stdio.h>
#include <string.h>
#include <hag/testing/mock.h>
#include <hag/drivers/vga/regs.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/attribc/index.h>

#if 0
#define VERBOSE(s)s;
#else
#define VERBOSE(s)
#endif

#if 1
#define PRINTRAW
#endif

uint8_t Internal_ReadPortByte(uint16_t reg);
#pragma aux Internal_ReadPortByte = \
    "in al, dx"                     \
    parm [dx]                       \
    value [al];

uint16_t Internal_ReadPortShort(uint16_t reg);
#pragma aux Internal_ReadPortShort =\
    "in ax, dx"                     \
    parm [dx]                       \
    value [ax];

void Internal_WritePortByte(uint16_t reg, uint8_t val);
#pragma aux Internal_WritePortByte =\
    "out dx, al"                    \
    parm [dx] [al];

namespace Hag { namespace Testing { namespace Mock { 


CustomPortHandler::~CustomPortHandler()
{

}

CustomPortHandler* CustomPortHandler::CheckTypeId(uint32_t id)
{
    return id == s_ID ? this : NULL;
}

struct Instance
{
    inline Instance()
        : Allocator(NULL)
        , PortHandlers(NULL)
        , PortMap(NULL)
        , Ports(NULL)
        , MemoryMap(NULL)
        , Memory(NULL)
        , DefaultMemory(NULL)
        , DefaultPortValues(NULL)
        , DefaultPortValuesCount(0)
    {

    }

    void Initialize(IAllocator& allocator, PortAndValue* defaultValues, uint16_t defaultValuesCount);
    void Reset();
    void Snapshot();
    void Rollback();

    void Shutdown();

    bool MarkPort(uint16_t port);

    void CachePort8(uint16_t port);
    void CachePort16(uint16_t port);

    void SetDefaultMemory(uint8_t* memory, uint32_t offset, uint32_t size);

    bool MarkMemory(uint32_t offset);

    uint8_t& CacheMemory(uint32_t offset, uint16_t size, uint16_t count);

    IAllocator* Allocator;
    CustomPortHandler* PortHandlers;
    uint8_t* PortMap; //1 bit for every port in a 64k address range.
    uint8_t* Ports;
    uint8_t* SnapshotPortMap;
    uint8_t* SnapshotPorts;

    uint8_t* MemoryMap;
    uint8_t* Memory;
    uint8_t* DefaultMemory;
    uint8_t* SnapshotMemory;

    PortAndValue* DefaultPortValues;
    uint16_t DefaultPortValuesCount;
};

void Instance::Initialize(IAllocator& allocator, PortAndValue* defaultValues, uint16_t defaultValuesCount)
{
    if (Allocator != NULL)
        Shutdown();

    Allocator = &allocator;

    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    Ports = allocator.AllocateAs<uint8_t>(portsSize);
    PortMap = allocator.AllocateAs<uint8_t>(portMapSize);//64Kib / 8 - One bit per byte.
    SnapshotPorts = allocator.AllocateAs<uint8_t>(portsSize);
    SnapshotPortMap = allocator.AllocateAs<uint8_t>(portMapSize);
    DefaultMemory = allocator.AllocateAs<uint8_t>(memorySize);
    memset(DefaultMemory, 0, memorySize);
    SnapshotMemory = allocator.AllocateAs<uint8_t>(memorySize);
    Memory = allocator.AllocateAs<uint8_t>(memorySize);
    MemoryMap = allocator.AllocateAs<uint8_t>(memoryMapSize);//1MiB / 8 - One bit per byte.

    DefaultPortValues = defaultValues;
    DefaultPortValuesCount = defaultValuesCount;

    Reset();
}

void Instance::Snapshot()
{
    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    for (uint32_t i = 0; i < portsSize; ++i)
    {
        uint16_t index = i >> 3;
        uint8_t bit = 1 << (i & 0x0007);
        if ((PortMap[index] & bit) != 0x00)
        {
            SnapshotPortMap[index] |= bit;
            SnapshotPorts[i] = Ports[i];
        }
    }
    memset(PortMap, 0, portMapSize);
    memset(Ports, 0, portsSize);

    for (uint32_t i = 0; i < memoryMapSize; ++i)
    {
        if (MemoryMap[i] != 0x00)
        {
            for (uint32_t j = 0; j < 8; ++j)
            {
                uint8_t bit = 1 << j;
                if ((MemoryMap[i] & bit) != 0x00)
                {
                    uint32_t index = (i << 3) + j;
                    SnapshotMemory[index] = Memory[index];
                }
            }
        }
    }
    memset(MemoryMap, 0, memoryMapSize);
    memset(Memory, 0, memorySize);

    CustomPortHandler* ptr = PortHandlers;
    while (ptr != NULL)
    {
        ptr->Snapshot();
        ptr = ptr->GetNext();
    }
}

void Instance::Rollback()
{
    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    memset(PortMap, 0, portMapSize);
    memset(Ports, 0, portsSize);
    memset(MemoryMap, 0, memoryMapSize);
    memset(Memory, 0, memorySize);

    CustomPortHandler* ptr = PortHandlers;
    while (ptr != NULL)
    {
        ptr->Rollback();
        ptr = ptr->GetNext();
    }
}

void Instance::Reset()
{
    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    memset(PortMap, 0, portMapSize);
    memset(Ports, 0, portsSize);
    memset(SnapshotPortMap, 0, portMapSize);
    memset(SnapshotPorts, 0, portsSize);
    memset(MemoryMap, 0, memoryMapSize);
    memset(Memory, 0, memorySize);
    memcpy(SnapshotMemory, DefaultMemory, memorySize);

    for (int i = 0; i < DefaultPortValuesCount; ++i)
    {
        uint16_t index = DefaultPortValues[i].Port >> 3;
        uint8_t bit = 1 << (DefaultPortValues[i].Port & 0x0007);
        SnapshotPortMap[index] |= bit;
        SnapshotPorts[DefaultPortValues[i].Port] = DefaultPortValues[i].Value;
    }

    CustomPortHandler* ptr = PortHandlers;
    while (ptr != NULL)
    {
        ptr->Reset();
        ptr = ptr->GetNext();
    }
}

void Instance::Shutdown()
{
    if (Allocator == NULL)
        return;

    while (PortHandlers != NULL)
    {
        CustomPortHandler* tmp = PortHandlers->GetNext();
        PortHandlers->~CustomPortHandler();
        Allocator->Free(PortHandlers);
        PortHandlers = tmp;
    }

    Allocator->Free(Ports);
    Ports = NULL;

    Allocator->Free(PortMap);
    PortMap = NULL;
    
    Allocator->Free(SnapshotPorts);
    SnapshotPorts = NULL;

    Allocator->Free(SnapshotPortMap);
    SnapshotPortMap = NULL;

    Allocator->Free(Memory);
    Memory = NULL;

    Allocator->Free(DefaultMemory);
    DefaultMemory = NULL;

    Allocator->Free(SnapshotMemory);

    Allocator->Free(MemoryMap);
    MemoryMap = NULL;

    Allocator = NULL;
}

bool Instance::MarkPort(uint16_t port)
{
    if (Allocator == NULL)
        return false;

    uint16_t index = port >> 3;
    uint8_t bit = 1 << (port & 0x0007);
    bool ret = (PortMap[index] & bit) == 0x00;
    PortMap[index] |= bit;
    return ret;
}

void Instance::CachePort8(uint16_t port)
{
    if (Allocator == NULL)
        return;

    uint16_t index = port >> 3;
    uint8_t bit = 1 << (port & 0x0007);
    if (MarkPort(port))
    {
        if ((SnapshotPortMap[index] & bit) != 0x00)
        {
            Ports[port] = SnapshotPorts[port];
        }
        else
        {
            Ports[port] = Internal_ReadPortByte(port);
            printf("Port 0x%04X not in default list.\n", port);
        }
    }
}

void Instance::CachePort16(uint16_t port)
{
    if (Allocator == NULL)
        return;
    
    CachePort8(port);
    CachePort8(port + 1);
}

void Instance::SetDefaultMemory(uint8_t* memory, uint32_t offset, uint32_t size)
{
    memcpy(DefaultMemory + offset, memory, size);
    memcpy(SnapshotMemory + offset, memory, size);
}

bool Instance::MarkMemory(uint32_t offset)
{
    if (Allocator == NULL)
        return false;

    uint32_t index = offset >> 3;
    uint8_t bit = 1 << (offset & 0x0007);
    bool ret = (MemoryMap[index] & bit) == 0x00;
    MemoryMap[index] |= bit;
    return ret;

}

uint8_t dummy[16] = { 0 };

uint8_t& Instance::CacheMemory(uint32_t offset, uint16_t size, uint16_t count)
{
    if (Allocator == NULL)
        return dummy[0];

    for (uint16_t i = 0; i < size * count; ++i)
    {
        if (MarkMemory(offset + i))
            Memory[offset + i] = SnapshotMemory[offset + i];
    }
    return Memory[offset];
}

Instance s_Instance0;
Instance s_Instance1;

Instance* s_CurrentInstance = &s_Instance0;

const uint32_t CustomPortHandler::s_ID = 0x62947393;

class ReadOnlyPort : public CustomPortHandler
{
public:
    inline ReadOnlyPort(const char* name, uint16_t port)
        : CustomPortHandler(name)
        , m_Port(port)
    {
    }

    virtual ~ReadOnlyPort()
    {
    }

    virtual bool CanHandle(uint16_t port)
    {
        return port == m_Port;
    }

    virtual bool HasHandled(uint16_t port)
    {
        return port == m_Port;
    }

    virtual uint8_t Read8(uint16_t port)
    {
        s_CurrentInstance->CachePort8(port);
        return s_CurrentInstance->Ports[port];
    }

    virtual uint16_t Read16(uint16_t port)
    {
        uint16_t lo = Hag::Testing::Mock::Port::Read8(port);
        uint16_t hi = Hag::Testing::Mock::Port::Read8(port + 1);
        return lo | (hi << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        //Intentionally left blank.
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        //drop low byte, forward hi byte.
        Hag::Testing::Mock::Port::Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        //drop low byte, forward hi byte.
        Hag::Testing::Mock::Port::Write8(port + 1, uint8_t(value >> 8));
    }

    virtual void Report(CustomPortHandler* instance1)
    {

    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        return false;
    }

    virtual void Reset()
    {
        //Nothing to do.
    }

    virtual void Snapshot()
    {
        //Nothing to do.
    }

    virtual void Rollback()
    {
        //Nothing to do.
    }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x0285fd7a;

private:
    uint16_t m_Port;
};

//I don't want to use a vector because that means I have to enable exceptions. bleh.
struct ReadOnlyReg
{
    uint8_t Reg;
    ReadOnlyReg* Next;
};

class IndexedPort : public CustomPortHandler
{
public:
    inline IndexedPort(IAllocator& allocator, const char* name, uint16_t indexPort, uint16_t dataPort, uint16_t regCount, uint8_t indexMask, uint8_t* defaultValues)
        : CustomPortHandler(name)
        , m_Allocator(&allocator)
        , m_IndexPort(indexPort)
        , m_DataPort(dataPort)
        , m_RegisterCount(regCount)
        , m_IndexMask(indexMask)
        , m_Data(NULL)
        , m_DataMask(NULL)
        , m_SnapshotValues(NULL)
        , m_DefaultValues(defaultValues)
        , m_ReadOnlyRegisters(NULL)
    {
        m_Data = m_Allocator->AllocateAs<uint8_t>(regCount);
        m_DataMask = m_Allocator->AllocateAs<uint8_t>(regCount >> 3);
        m_SnapshotValues = m_Allocator->AllocateAs<uint8_t>(regCount);
        Reset();
    }

    virtual ~IndexedPort()
    {
        m_Allocator->Free(m_DataMask);
        m_DataMask = NULL;

        m_Allocator->Free(m_Data);
        m_Data = NULL;

        m_Allocator->Free(m_SnapshotValues);
        m_SnapshotValues = NULL;

        while (m_ReadOnlyRegisters != NULL)
        {
            ReadOnlyReg* reg = m_ReadOnlyRegisters;
            m_ReadOnlyRegisters = reg->Next;
            m_Allocator->Free(reg);
        }
    }

    virtual void Reset()
    {
        uint16_t dataMaskCount = m_RegisterCount >> 3;
        if ((m_RegisterCount & 0x7) != 0x00)
            ++dataMaskCount;

        memset(m_Data, 0, m_RegisterCount);
        memset(m_DataMask, 0, dataMaskCount);
        memcpy(m_SnapshotValues, m_DefaultValues, m_RegisterCount);
    }

    virtual void Snapshot()
    {
        for (uint32_t i = 0; i < m_RegisterCount; ++i)
        {
            uint32_t blockIndex = i >> 3;
            uint8_t bitMask = 1 << (i & 0x07);
            if ((m_DataMask[blockIndex] & bitMask) != 0x00)
            {
                m_SnapshotValues[i] = m_Data[i];
            }
        }

        uint16_t dataMaskCount = m_RegisterCount >> 3;
        if ((m_RegisterCount & 0x7) != 0x00)
            ++dataMaskCount;

        memset(m_Data, 0, m_RegisterCount);
        memset(m_DataMask, 0, dataMaskCount);
    }

    virtual void Rollback()
    {
        memset(m_Data, 0, m_RegisterCount);

        uint16_t dataMaskCount = m_RegisterCount >> 3;
        if ((m_RegisterCount & 0x7) != 0x00)
            ++dataMaskCount;

        memset(m_DataMask, 0, dataMaskCount);
    }

    virtual bool CanHandle(uint16_t port)
    {
        return (port == m_IndexPort) || (port == m_DataPort);
    }

    virtual bool HasHandled(uint16_t port)
    {
        return (port == m_IndexPort) || (port == m_DataPort);
    }

    virtual uint8_t Read8(uint16_t port)
    {
        if (port == m_DataPort)
        {
            uint8_t index = GetIndex();
            if (index >= m_RegisterCount)
                return 0;

            uint8_t blockIndex = index >> 3;
            uint8_t bitMask = 1 << (index & 0x07);
            
            if ((m_DataMask[blockIndex] & bitMask) == 0x00)
            {
                m_DataMask[blockIndex] |= bitMask;
                m_Data[index] = m_SnapshotValues[index];
            }
            VERBOSE(printf("%s port read 0x%04X:0x%02X = 0x%02X\n", GetName(), port, index, m_Data[index]));
            return m_Data[index];
        }
        else if (port == m_IndexPort)
        {
            s_CurrentInstance->CachePort8(m_IndexPort);
            VERBOSE(printf("%s port read 0x%04X = 0x%02X\n", GetName(), port, s_CurrentInstance->Ports[m_IndexPort]));
            return s_CurrentInstance->Ports[m_IndexPort];
        }
        return 0;
    }

    virtual uint16_t Read16(uint16_t port)
    {
        return Read8(port) | (uint16_t(Read8(port + 1)) << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        if (port == m_DataPort)
        {
            uint8_t index = GetIndex();
            if (index >= m_RegisterCount)
                return;
            
            ReadOnlyReg* ptr = m_ReadOnlyRegisters;
            while (ptr != NULL)
            {
                if (index == ptr->Reg)
                    return;

                ptr = ptr->Next;
            }

            uint8_t blockIndex = index >> 3;
            uint8_t bitMask = 1 << (index & 0x07);
            m_DataMask[blockIndex] |= bitMask;
            m_Data[index] = value;           
            VERBOSE(printf("%s port write 0x%04X:0x%02X = 0x%02X\n", GetName(), port, index, value));
        }
        else if (port == m_IndexPort)
        {
            s_CurrentInstance->MarkPort(m_IndexPort);
            s_CurrentInstance->Ports[m_IndexPort] = value;
            VERBOSE(printf("%s port write 0x%04X = 0x%02X\n", GetName(), port, value));
        }
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        Write8(port, valueLo);
        Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        Write8(port, uint8_t(value));
        Write8(port + 1, uint8_t(value >> 8));
    }

    void FetchModifiedIndexedRegisters(IndexedRegisterCheckCallback_t callback, void* context)
    {
        for (uint32_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((m_DataMask[index] & bits) != 0)
            {
                uint8_t originalValue = m_SnapshotValues[i];
                callback(m_DataPort, uint8_t(i), m_Data[i], originalValue, context);
            }
        }
    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        IndexedPort* inst0 = this;
        IndexedPort* inst1 = (IndexedPort*)instance1;
        for (uint16_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((inst0->m_DataMask[index] & bits) !=
                (inst1->m_DataMask[index] & bits))
            {
                return true;
            }
            else if ((inst0->m_DataMask[index] & bits) != 0 &&
                     (inst1->m_DataMask[index] & bits) != 0)
            {
                if (inst0->m_Data[i] != inst1->m_Data[i])
                {
                    return true;
                }
            }
        }

        return false;
    }

    virtual void Report(CustomPortHandler* instance1)
    {
        printf("\n%s report:\n", GetName());
        IndexedPort* inst0 = this;
        IndexedPort* inst1 = (IndexedPort*)instance1;
        for (uint16_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((inst0->m_DataMask[index] & bits) !=
                (inst1->m_DataMask[index] & bits))
            {
                if ((inst0->m_DataMask[index] & bits) != 0)
                {
                    printf("Only instance 0 has content in register 0x%02X = 0x%02X\n", i, inst0->m_Data[i]);
                }

                if ((inst1->m_DataMask[index] & bits) != 0)
                {
                    printf("Only instance 1 has content in register 0x%02X = 0x%02X\n", i, inst1->m_Data[i]);
                }
            }
            else if ((inst0->m_DataMask[index] & bits) != 0 &&
                     (inst1->m_DataMask[index] & bits) != 0)
            {
                if (inst0->m_Data[i] != inst1->m_Data[i])
                {
                    printf("Content of register 0x%02X differs: 0x%02X != 0x%02X\n", i, inst0->m_Data[i], inst1->m_Data[i]);
                }
            }
        }
    }

    inline void AddReadOnlyRegister(uint8_t reg)
    {
        ReadOnlyReg* ptr = ::new(m_Allocator->Allocate(sizeof(ReadOnlyReg))) ReadOnlyReg();
        ptr->Next = m_ReadOnlyRegisters;
        m_ReadOnlyRegisters = ptr;
        ptr->Reg = reg;
    }

    inline uint16_t GetIndexPort() {return m_IndexPort; }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x23b2a89f;

private:
    IAllocator* m_Allocator;
    uint16_t m_IndexPort;
    uint16_t m_DataPort;
    uint16_t m_RegisterCount;
    uint8_t m_IndexMask;
    uint8_t* m_Data;
    uint8_t* m_DataMask;
    uint8_t* m_SnapshotValues;
    uint8_t* m_DefaultValues;
    ReadOnlyReg* m_ReadOnlyRegisters;

    uint8_t GetIndex()
    {
        s_CurrentInstance->CachePort8(m_IndexPort);
        return s_CurrentInstance->Ports[m_IndexPort] & m_IndexMask;
    }
};

class AttributePortHandler : public CustomPortHandler
{
public:
    AttributePortHandler(IAllocator& allocator, uint8_t* defaultValues)
        : CustomPortHandler("Attribute Ports")
        , m_Allocator(allocator)
        , Mask(NULL)
        , Data(NULL)
        , SnapshotValues(NULL)
        , DefaultValues(defaultValues)
        , m_OriginalIndex(0)
        , m_CurrentIndex(0)
        , m_IsIndex(true)
    {
        m_OriginalIndex = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
        m_CurrentIndex = m_OriginalIndex;

        uint8_t size = 32;
        Mask = m_Allocator.AllocateAs<uint8_t>(size >> 3);
        Data = m_Allocator.AllocateAs<uint8_t>(size);
        SnapshotValues = m_Allocator.AllocateAs<uint8_t>(size);
        
        Reset();
    }

    virtual ~AttributePortHandler()
    {
        if (Mask != NULL)
            m_Allocator.Free(Mask);
        Mask = NULL;

        if (Data != NULL)
            m_Allocator.Free(Data);
        Data = NULL;

        if (SnapshotValues != NULL)
            m_Allocator.Free(SnapshotValues);
        SnapshotValues = NULL;
    }

    virtual void Reset()
    {
        memset(Mask, 0, 32 >> 3);
        memset(Data, 0, 32);
        memcpy(SnapshotValues, DefaultValues, 32);
    }

    virtual void Snapshot()
    {
        for (uint8_t i = 0; i < 32; ++i)
        {
            uint8_t blockIndex = i >> 3;
            uint8_t bits = 1 << (i & 0x07);
            if ((Mask[blockIndex] & bits) != 0x00)
            {
                SnapshotValues[i] = Data[i];
            }
        }
        memset(Mask, 0, 32 >> 3);
        memset(Data, 0, 32);
    }

    virtual void Rollback()
    {
        memset(Mask, 0, 32 >> 3);
        memset(Data, 0, 32);
    }

    bool MarkPort(uint8_t index)
    {
        uint8_t blockIndex = index >> 3;
        uint8_t bits = 1 << (index & 0x07);
        bool ret = (Mask[blockIndex] & bits) == 0x00;
        Mask[blockIndex] |= bits;
        return ret;
    }

    void CacheRegister(uint8_t index)
    {
        if (MarkPort(index))
        {
            Data[index] = SnapshotValues[index];
        }
    }

    virtual bool CanHandle(uint16_t port)
    {
        return port == Hag::VGA::Register::InputStatus1B ||
               port == Hag::VGA::Register::InputStatus1D ||
               port == Hag::VGA::Register::AttributeControllerIndex ||
               port == Hag::VGA::Register::AttributeControllerDataR;
    }

    virtual bool HasHandled(uint16_t port)
    {
        return port == Hag::VGA::Register::AttributeControllerIndex ||
               port == Hag::VGA::Register::AttributeControllerDataR;
    }

    virtual uint8_t Read8(uint16_t port)
    {
        if (port == Hag::VGA::Register::InputStatus1B ||
            port == Hag::VGA::Register::InputStatus1D)
        {
            m_IsIndex = true;
            return 0;
        }

        if (port == Hag::VGA::Register::AttributeControllerIndex)
        {
            return m_CurrentIndex;
        }

        if (port == Hag::VGA::Register::AttributeControllerDataR)
        {
            uint8_t index = m_CurrentIndex & 0x1f;
            CacheRegister(index);
            return Data[index];
        }

        return 0;
    }

    virtual uint16_t Read16(uint16_t port)
    {
        uint16_t lo = Hag::Testing::Mock::Port::Read8(port);
        uint16_t hi = Hag::Testing::Mock::Port::Read8(port + 1);
        return lo | (hi << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        if (port == Hag::VGA::Register::AttributeControllerDataW)
        {
            if (m_IsIndex)
            {
                m_CurrentIndex = value;
            }
            else
            {
                uint8_t index = m_CurrentIndex & 0x1f;
                MarkPort(index);
                Data[index] = value;
            }
            m_IsIndex = !m_IsIndex;
        }
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        Hag::Testing::Mock::Port::Write8(port, valueLo);
        Hag::Testing::Mock::Port::Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        Hag::Testing::Mock::Port::Write8(port, uint8_t(value));
        Hag::Testing::Mock::Port::Write8(port + 1, uint8_t(value >> 8));
    }

    void FetchModifiedIndexedRegisters(IndexedRegisterCheckCallback_t callback, void* context)
    {
        for (uint32_t i = 0; i < 32; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((Mask[index] & bits) != 0)
            {
                uint8_t originalValue = SnapshotValues[i];
                callback(Hag::VGA::Register::AttributeControllerDataW, uint8_t(i), Data[i], originalValue, context);
            }
        }
    }

    virtual void Report(CustomPortHandler* instance1)
    {

    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        return false;
    }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x5d038b73;

private:
    Hag::IAllocator& m_Allocator;
    uint8_t* Mask;
    uint8_t* Data;
    uint8_t* SnapshotValues;
    uint8_t* DefaultValues;
    uint8_t m_OriginalIndex;
    uint8_t m_CurrentIndex;
    bool m_IsIndex;
};

class RAMDACPortHandler : public CustomPortHandler
{
public:
    RAMDACPortHandler(IAllocator& allocator, uint8_t* defaultValues)
        : CustomPortHandler("RAMDAC Ports")
        , m_Allocator(allocator)
        , Mask(NULL)
        , Data(NULL)
        , SnapshotValues(NULL)
        , DefaultValues(defaultValues)
        , ReadIndex(0)
        , WriteIndex(0)
    {
        uint16_t size = 256 * 3;
        Mask = m_Allocator.AllocateAs<uint8_t>(size >> 3);
        Data = m_Allocator.AllocateAs<uint8_t>(size);
        SnapshotValues = m_Allocator.AllocateAs<uint8_t>(size);
        
        Reset();
    }

    virtual ~RAMDACPortHandler()
    {
        if (Mask != NULL)
            m_Allocator.Free(Mask);
        Mask = NULL;

        if (Data != NULL)
            m_Allocator.Free(Data);
        Data = NULL;

        if (SnapshotValues != NULL)
            m_Allocator.Free(SnapshotValues);
        SnapshotValues = NULL;
    }

    virtual void Reset()
    {
        uint16_t size = 256 * 3;
        memset(Mask, 0, size >> 3);
        memset(Data, 0, size);
        memcpy(SnapshotValues, DefaultValues, size);
    }

    virtual void Snapshot()
    {
        uint16_t size = 256 * 3;
        for (uint16_t i = 0; i < size; ++i)
        {
            uint16_t blockIndex = i >> 3;
            uint8_t bits = 1 << (i & 0x07);
            if ((Mask[blockIndex] & bits) != 0x00)
            {
                SnapshotValues[i] = Data[i];
            }
        }
        memset(Mask, 0, size >> 3);
        memset(Data, 0, size);
    }

    virtual void Rollback()
    {
        uint16_t size = 256 * 3;
        memset(Mask, 0, size >> 3);
        memset(Data, 0, size);
    }

    bool MarkPort(uint16_t index)
    {
        uint16_t blockIndex = index >> 3;
        uint8_t bits = uint8_t(1 << (index & 0x07));
        bool ret = (Mask[blockIndex] & bits) == 0x00;
        Mask[blockIndex] |= bits;
        return ret;
    }

    void CacheRegister(uint16_t index)
    {
        if (MarkPort(index))
        {
            Data[index] = SnapshotValues[index];
        }
    }

    virtual bool CanHandle(uint16_t port)
    {
        return port == Hag::VGA::Register::DACReadIndex ||
               port == Hag::VGA::Register::DACWriteIndex ||
               port == Hag::VGA::Register::RAMDACData;
    }

    virtual bool HasHandled(uint16_t port)
    {
        return port == Hag::VGA::Register::DACReadIndex ||
               port == Hag::VGA::Register::DACWriteIndex ||
               port == Hag::VGA::Register::RAMDACData;
    }

    virtual uint8_t Read8(uint16_t port)
    {
        if (port == Hag::VGA::Register::RAMDACData)
        {
            CacheRegister(ReadIndex);
            uint8_t ret = Data[ReadIndex];
            ++ReadIndex;
            return ret;
        }
        
        if (port == Hag::VGA::Register::DACReadIndex)
        {
            return uint8_t(ReadIndex / 3);
        }

        if (port == Hag::VGA::Register::DACWriteIndex)
        {
            return uint8_t(WriteIndex / 3);
        }

        return 0;
    }

    virtual uint16_t Read16(uint16_t port)
    {
        uint16_t lo = Hag::Testing::Mock::Port::Read8(port);
        uint16_t hi = Hag::Testing::Mock::Port::Read8(port + 1);
        return lo | (hi << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        if (port == Hag::VGA::Register::RAMDACData)
        {
            MarkPort(WriteIndex);
            Data[WriteIndex] = value;
            ++WriteIndex;
        }
        
        if (port == Hag::VGA::Register::DACReadIndex)
        {
            ReadIndex = value * 3;
        }

        if (port == Hag::VGA::Register::DACWriteIndex)
        {
            WriteIndex = value * 3;
        }
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        Hag::Testing::Mock::Port::Write8(port, valueLo);
        Hag::Testing::Mock::Port::Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        Hag::Testing::Mock::Port::Write8(port, uint8_t(value));
        Hag::Testing::Mock::Port::Write8(port + 1, uint8_t(value >> 8));
    }

    void FetchModifiedIndexedRegisters(IndexedRegisterCheckCallback_t callback, void* context)
    {
        uint16_t size = 256 * 3;
        for (uint32_t i = 0; i < size; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((Mask[index] & bits) != 0)
            {
                uint8_t originalValue = SnapshotValues[i];
                callback(Hag::VGA::Register::DACReadIndex, uint16_t(i), Data[i], originalValue, context);
            }
        }
    }

    virtual void Report(CustomPortHandler* instance1)
    {

    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        return false;
    }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x275b2e8f;

private:
    Hag::IAllocator& m_Allocator;
    uint8_t* Mask;
    uint8_t* Data;
    uint8_t* SnapshotValues;
    uint8_t* DefaultValues;
    uint16_t ReadIndex;
    uint16_t WriteIndex;
};

void Initialize(IAllocator& allocator, PortAndValue* defaultPortsAndValues, uint16_t defaultPortsAndValuesCount, uint8_t* attributeControllerRegisters, uint8_t* ramdacRegisters)
{
    s_Instance0.Initialize(allocator, defaultPortsAndValues, defaultPortsAndValuesCount);
    s_Instance1.Initialize(allocator, defaultPortsAndValues, defaultPortsAndValuesCount);

    AttributePortHandler* attr0 = ::new(s_Instance0.Allocator->Allocate(sizeof(AttributePortHandler))) 
        AttributePortHandler(allocator, attributeControllerRegisters);
    attr0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = attr0;

    AttributePortHandler* attr1 = ::new(s_Instance0.Allocator->Allocate(sizeof(AttributePortHandler))) 
        AttributePortHandler(allocator, attributeControllerRegisters);
    attr1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = attr1;

    RAMDACPortHandler* ramdac0 = ::new(s_Instance0.Allocator->Allocate(sizeof(RAMDACPortHandler))) 
        RAMDACPortHandler(allocator, ramdacRegisters);
    ramdac0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = ramdac0;

    RAMDACPortHandler* ramdac1 = ::new(s_Instance0.Allocator->Allocate(sizeof(RAMDACPortHandler))) 
        RAMDACPortHandler(allocator, ramdacRegisters);
    ramdac1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = ramdac1;
}

void AddReadOnlyPort(const char* name, uint16_t port)
{
    if (s_Instance0.Allocator == NULL)
        return;

    ReadOnlyPort* port0 = ::new(s_Instance0.Allocator->Allocate(sizeof(ReadOnlyPort))) 
        ReadOnlyPort(name, port);
    port0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = port0;

    ReadOnlyPort* port1 = ::new(s_Instance1.Allocator->Allocate(sizeof(ReadOnlyPort))) 
        ReadOnlyPort(name, port);
    port1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = port1;
}

void AddIndexedPort(const char* name, uint16_t indexPort, uint8_t indexMask, uint16_t dataPort, uint16_t regCount, uint8_t* defaultValues)
{
    if (s_Instance0.Allocator == NULL)
        return;

    IndexedPort* port0 = ::new(s_Instance0.Allocator->Allocate(sizeof(IndexedPort))) 
        IndexedPort(*s_Instance0.Allocator, name, indexPort, dataPort, regCount, indexMask, defaultValues);
    port0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = port0;

    IndexedPort* port1 = ::new(s_Instance1.Allocator->Allocate(sizeof(IndexedPort))) 
        IndexedPort(*s_Instance1.Allocator, name, indexPort, dataPort, regCount, indexMask, defaultValues);
    port1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = port1;
}

void AddReadOnlyIndexedRegister(uint16_t port, uint8_t reg)
{
    CustomPortHandler* ptr = s_Instance0.PortHandlers;
    while (ptr != NULL)
    {
        IndexedPort* idxPort = customporthandler_cast<IndexedPort>(ptr);
        if (idxPort != NULL && idxPort->GetIndexPort() == port)
        {
            idxPort->AddReadOnlyRegister(reg);
            break;
        }
        ptr = ptr->GetNext();
    }

    ptr = s_Instance1.PortHandlers;
    while (ptr != NULL)
    {
        IndexedPort* idxPort = customporthandler_cast<IndexedPort>(ptr);
        if (idxPort != NULL && idxPort->GetIndexPort())
        {
            idxPort->AddReadOnlyRegister(reg);
            break;
        }
        ptr = ptr->GetNext();
    }
}

void SetDefaultMemory(uint8_t* memory, uint32_t offset, uint32_t size)
{
    if (s_Instance0.Allocator == NULL)
        return;

    if (offset + size > 1024 * 1024)
        return;

    s_Instance0.SetDefaultMemory(memory, offset, size);
    s_Instance1.SetDefaultMemory(memory, offset, size);
}

struct VerifyPaVContext
{
    PortAndValue* modifiedPortsAndValues;
    uint16_t* readPortsAndValues;
    PortAndIndexAndValue* modifiedIndexedPortsAndValues;
    PortAndIndex* readIndexedPortsAndValues;
    uint16_t* ignorePorts;
    PortAndIndex* ignoreIndexedPorts;
    int modifiedCount;
    int readCount;
    int modifiedIndexedCount;
    int readIndexedCount;
    int ignorePortsCount;
    int ignoreIndexedCount;
    int verifiedCount;
};

void VerifyPortsAndValuesCallback(uint16_t port, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyPaVContext& context = *(VerifyPaVContext*)ctx;
    bool found = false;

    for (int i = 0; i < context.ignorePortsCount; ++i)
    {
        if (context.ignorePorts[i] == port)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        for (int i = 0; i < context.modifiedCount; ++i)
        {
            if (context.modifiedPortsAndValues[i].Port == port)
            {
                if (context.modifiedPortsAndValues[i].Value == modifiedValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X modified value should be 0x%02X but is 0x%02X\n", port, context.modifiedPortsAndValues[i].Value, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        for (int i = 0; i < context.readCount; ++i)
        {
            if (context.readPortsAndValues[i] == port)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X marked read, value should be 0x%02X but is 0x%02X\n", port, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
#ifndef PRINTRAW
        printf("Port 0x%04X not found in test data. Original: 0x%02X", port, originalValue);
#endif
        if (originalValue != modifiedValue)
        {
#ifdef PRINTRAW
            printf("{ 0x%04X, 0x%02X },\n", port, modifiedValue);
#else
            printf(", modified: 0x%02X\n", modifiedValue);
#endif
        }
        else
        {
#ifdef PRINTRAW
            printf("0x%04X,\n", port);
#else
            printf("\n");
#endif
        }
    }
}

void VerifyPortsAndValuesIndexedCallback(uint16_t port, uint16_t index, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyPaVContext& context = *(VerifyPaVContext*)ctx;
    bool found = false;

    for (int i = 0; i < context.ignoreIndexedCount; ++i)
    {
        if (context.ignoreIndexedPorts[i].Port == port &&
            context.ignoreIndexedPorts[i].Index == index)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        for (int i = 0; i < context.modifiedIndexedCount; ++i)
        {
            if (context.modifiedIndexedPortsAndValues[i].Port == port &&
                context.modifiedIndexedPortsAndValues[i].Index == index)
            {
                if (context.modifiedIndexedPortsAndValues[i].Value == modifiedValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X:0x%02X modified value should be 0x%02X but is 0x%02X\n", port, index, context.modifiedIndexedPortsAndValues[i].Value, modifiedValue);
                }
                break;
            }
        }
    }
    if (!found)
    {
        for (int i = 0; i < context.readIndexedCount; ++i)
        {
            if (context.readIndexedPortsAndValues[i].Port == port &&
                context.readIndexedPortsAndValues[i].Index == index)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X:0x%02X marked read, value should be 0x%02X but is 0x%02X\n", port, index, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
#ifndef PRINTRAW
        printf("Port 0x%04X:0x%02X not found in test data. Original: 0x%02X", port, index, originalValue);
#endif
        if (originalValue != modifiedValue)
        {
#ifdef PRINTRAW
            printf("{ 0x%04X, 0x%04X, 0x%02X },\n", port, index, modifiedValue);
#else
            printf(", modified: 0x%02X\n", modifiedValue);
#endif
        }
        else
        {
#ifdef PRINTRAW
            printf("{ 0x%04X, 0x%04X },\n", port, index);
#else
            printf("\n");
#endif
        }
    }
}

int VerifyPortsAndValues(int instance, PortAndValue* modifiedPortsAndValues, int modifiedCount,
                         uint16_t* readPortsAndValues, int readCount,
                         PortAndIndexAndValue* modifiedIndexedPortsAndValues, int modifiedIndexedCount,
                         PortAndIndex* readIndexedPortsAndValues, int readIndexedCount,
                         uint16_t* ignorePorts, int ignorePortsCount,
                         PortAndIndex* ignoreIndexedPorts, int ignoreIndexedCount)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    VerifyPaVContext context = 
    {
        modifiedPortsAndValues,
        readPortsAndValues,
        modifiedIndexedPortsAndValues,
        readIndexedPortsAndValues,
        ignorePorts,
        ignoreIndexedPorts,
        modifiedCount,
        readCount,
        modifiedIndexedCount,
        readIndexedCount,
        ignorePortsCount,
        ignoreIndexedCount,
        0
    };

    FetchModifiedRegisters(instance, VerifyPortsAndValuesCallback, &context);
#ifdef PRINTRAW
    //printf("\n");
#endif
    FetchModifiedIndexedRegisters(instance, VerifyPortsAndValuesIndexedCallback, &context);

    return context.verifiedCount;
}

void FetchModifiedIndexedRegisters(int instance, IndexedRegisterCheckCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    Instance* inst = instance == 0 ? &s_Instance0 : &s_Instance1;
    
    CustomPortHandler* ptr = inst->PortHandlers;
    while (ptr != NULL)
    {
        IndexedPort* indexed = customporthandler_cast<IndexedPort>(ptr);
        if (indexed != NULL)
        {
            indexed->FetchModifiedIndexedRegisters(callback, context);
        }

        AttributePortHandler* attr = customporthandler_cast<AttributePortHandler>(ptr);
        if (attr != NULL)
        {
            attr->FetchModifiedIndexedRegisters(callback, context);
        }

        RAMDACPortHandler* ramdac = customporthandler_cast<RAMDACPortHandler>(ptr);
        if (ramdac != NULL)
        {
            ramdac->FetchModifiedIndexedRegisters(callback, context);
        }

        ptr = ptr->GetNext();
    }
}

void FetchModifiedRegisters(int instance, RegisterCheckCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    Instance* inst = instance == 0 ? &s_Instance0 : &s_Instance1;
    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((inst->PortMap[blockIndex] & bits) != 0x00)
        {
            uint8_t orgValue = 0xFF;
            bool found = false;

            if ((inst->SnapshotPortMap[blockIndex] & bits) != 0x00)
            {
                orgValue = inst->SnapshotPorts[i];
                found = true;
            }

            //Hack
            if (i == Hag::VGA::Register::AttributeControllerIndex ||
                i == Hag::VGA::Register::AttributeControllerDataR ||
                i == Hag::VGA::Register::GraphicsControllerIndex ||
                i == Hag::VGA::Register::GraphicsControllerData ||
                i == Hag::VGA::Register::CRTControllerIndexB ||
                i == Hag::VGA::Register::CRTControllerIndexD ||
                i == Hag::VGA::Register::CRTControllerDataB ||
                i == Hag::VGA::Register::CRTControllerDataD)
                continue;

            if (!found)
            {
                printf("Was not able to locate default value for port 0x%04X\n", i);
            }
            //uint8_t orgValue = Internal_ReadPortByte(uint16_t(i));
            callback(uint16_t(i), inst->Ports[i], orgValue, context);
        }
    }
}

struct VerifyBDAContext
{
    BDAFieldsAndValues* modifiedFieldsAndValues;
    int modifiedCount;
    uint8_t* readFields;
    int readCount;
    uint8_t* ignoreFields;
    int ignoreCount;
    int verifiedCount;
};

void VerifyBDAFieldsCallback(uint8_t field, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyBDAContext& context = *(VerifyBDAContext*)ctx;

    bool found = false;

    for (uint32_t i = 0; i < context.ignoreCount; ++i)
    {
        if (context.ignoreFields[i] == field)
        {
            found = true;
            context.verifiedCount++;
            break;
        }
    }

    if (!found)
    {
        for (uint32_t i = 0; i < context.modifiedCount; ++i)
        {
            if (context.modifiedFieldsAndValues[i].Field == field)
            {
                if (context.modifiedFieldsAndValues[i].Value == modifiedValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("BDA field 0x%02X modified value should be 0x%02X but is 0x%02X\n", field, 
                        context.modifiedFieldsAndValues[i].Value, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        for (uint32_t i = 0; i < context.readCount; ++i)
        {
            if (context.readFields[i] == field)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("BDA field 0x%02X marked read, value should be 0x%02X but is 0x%02X\n", field, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        printf("BDA field 0x%02X not found in test data. Original: 0x%02X", field, originalValue);
        if (originalValue != modifiedValue)
        {
            printf(", modified: 0x%02X\n", modifiedValue);
        }
        else
        {
            printf("\n");
        }
    }

}

int VerifyBDAFieldsAndValues(int instance, BDAFieldsAndValues* modifiedFieldsAndValues, int modifiedCount,
                             uint8_t* readFields, int readCount, uint8_t* ignoreFields, int ignoreCount)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    VerifyBDAContext context =
    {
        modifiedFieldsAndValues,
        modifiedCount,
        readFields,
        readCount,
        ignoreFields,
        ignoreCount,
        0
    };

    FetchModifiedBDAFields(0, VerifyBDAFieldsCallback, &context);

    return context.verifiedCount;
}

void FetchModifiedBDAFields(int instance, BDAFieldCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    //uint8_t* realMem = (uint8_t*)0x00000000;
    for (uint32_t i = 0x400; i < 0x500; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_CurrentInstance->MemoryMap[blockIndex] & bits) != 0x00)
        {
            callback(i,  s_CurrentInstance->Memory[i], s_CurrentInstance->SnapshotMemory[i], context);
        }
    }
}

void SelectInstance(int instance)
{
    s_CurrentInstance = (instance & 1) == 0 ? &s_Instance0 : &s_Instance1;
}

void Snapshot()
{
    s_Instance0.Snapshot();
    s_Instance1.Snapshot();
}

void Rollback()
{
    s_Instance0.Rollback();
    s_Instance1.Rollback();
}

void Reset()
{
    s_Instance0.Reset();
    s_Instance1.Reset();
}

bool HasDifferences()
{
    if (s_Instance0.Allocator == NULL)
        return false;

    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_Instance0.PortMap[blockIndex] & bits) !=
            (s_Instance1.PortMap[blockIndex] & bits))
        {
            return true;
        }
        else if ((s_Instance0.PortMap[blockIndex] & bits) != 0 &&
                 (s_Instance1.PortMap[blockIndex] & bits) != 0)
        {
            if (s_Instance0.Ports[i] != s_Instance1.Ports[i])
            {
                return true;
            }
        }
    }

    CustomPortHandler* ptr0 = s_Instance0.PortHandlers;
    CustomPortHandler* ptr1 = s_Instance1.PortHandlers;
    while (ptr0 != NULL && ptr1 != NULL)
    {
        if (ptr0->HasDifferences(ptr1))
            return true;
        ptr0 = ptr0->GetNext();
        ptr1 = ptr1->GetNext();
    }

    for (uint32_t i = 0; i < (1024 * 1024) >> 3; ++i)
    {
        if (s_Instance0.MemoryMap[i] != 0 ||
            s_Instance1.MemoryMap[i] != 0)
        {
            for (uint32_t j = 0; j < 8; ++j)
            {
                uint8_t bits = uint8_t(1 << j);
                if ((s_Instance0.MemoryMap[i] & bits) !=
                    (s_Instance1.MemoryMap[i] & bits))
                {
                    return true;
                }
                else if ((s_Instance0.MemoryMap[i] & bits) != 0 &&
                    (s_Instance1.MemoryMap[i] & bits) != 0)
                {
                    if (s_Instance0.Memory[(i << 3) + j] !=
                        s_Instance1.Memory[(i << 3) + j])
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Report()
{
    if (s_Instance0.Allocator == NULL)
        return;

    printf("\nDifference report: \n");

    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_Instance0.PortMap[blockIndex] & bits) !=
            (s_Instance1.PortMap[blockIndex] & bits))
        {
            if (s_Instance0.PortMap[blockIndex] & bits)
            {
                printf("Only instance 0 has content in port 0x%04X = 0x%02X\n", i, s_Instance0.Ports[i]);
            }
            if (s_Instance1.PortMap[blockIndex] & bits)
            {
                printf("Only instance 1 has content in port 0x%04X = 0x%02X\n", i, s_Instance1.Ports[i]);
            }
        }
        else if ((s_Instance0.PortMap[blockIndex] & bits) != 0 &&
                 (s_Instance1.PortMap[blockIndex] & bits) != 0)
        {
            if (s_Instance0.Ports[i] != s_Instance1.Ports[i])
            {
                printf("Content of register 0x%02X differs: 0x%02X != 0x%02X\n", i, s_Instance0.Ports[i], s_Instance1.Ports[i]);
            }
        }
    }

    CustomPortHandler* ptr0 = s_Instance0.PortHandlers;
    CustomPortHandler* ptr1 = s_Instance1.PortHandlers;
    while (ptr0 != NULL && ptr1 != NULL)
    {
        ptr0->Report(ptr1);
        ptr0 = ptr0->GetNext();
        ptr1 = ptr1->GetNext();
    }

    uint8_t* realmem = 0x00000000;

    printf("\nMemory report:\n");
    for (uint32_t i = 0; i < (1024 * 1024) >> 3; ++i)
    {
        if (s_Instance0.MemoryMap[i] != 0 ||
            s_Instance1.MemoryMap[i] != 0)
        {
            for (uint32_t j = 0; j < 8; ++j)
            {
                uint8_t bits = uint8_t(1 << j);
                if ((s_Instance0.MemoryMap[i] & bits) !=
                    (s_Instance1.MemoryMap[i] & bits))
                {
                    if ((s_Instance0.MemoryMap[i] & bits) != 0)
                    {
                        printf("Only Instance 0 has content in memory offset 0x%05X = 0x%02X",
                            (i << 3) + j, s_Instance0.Memory[(i << 3) + j]);
                        if (realmem[(i << 3) + j] != s_Instance0.Memory[(i << 3) + j])
                        {
                            printf(" original: 0x%02X\n", realmem[(i << 3) + j]);
                        }
                        else
                        {
                            printf("\n");
                        }
                    }
                    if ((s_Instance1.MemoryMap[i] & bits) != 0)
                    {
                        printf("Only Instance 1 has content in memory offset 0x%05X = 0x%02X",
                            (i << 3) + j, s_Instance1.Memory[(i << 3) + j]);
                        if (realmem[(i << 3) + j] != s_Instance1.Memory[(i << 3) + j])
                        {
                            printf(" original: 0x%02X\n", realmem[(i << 3) + j]);
                        }
                        else
                        {
                            printf("\n");
                        }
                    }
                }
                else if ((s_Instance0.MemoryMap[i] & bits) != 0 &&
                    (s_Instance1.MemoryMap[i] & bits) != 0)
                {
                    if (s_Instance0.Memory[(i << 3) + j] !=
                        s_Instance1.Memory[(i << 3) + j])
                    {
                        printf("Content of memory offset 0x%05X differs: 0x%02X != 0x%02X original: 0x%02X\n", (i << 3) + j,
                            s_Instance0.Memory[(i << 3) + j], s_Instance1.Memory[(i << 3) + j], realmem[(i << 3) + j]);
                    }
                }
            }
        }
    }
}

int Shutdown()
{
    s_Instance1.Shutdown();
    s_Instance0.Shutdown();
    return 0;
}

namespace Port
{

uint8_t Read8(uint16_t port)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    uint8_t ret = 0;
    bool handled = false;
    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            ret = handler->Read8(port);
            if (handler->HasHandled(port))
            {
                handled = true;
                break;
            }
        }
        handler = handler->GetNext();
    }

    if (handled)
    {
        s_CurrentInstance->MarkPort(port);
        s_CurrentInstance->Ports[port] = ret;
    }
    else
    {
        s_CurrentInstance->CachePort8(port);
        ret = s_CurrentInstance->Ports[port];
        VERBOSE(printf("Port read 0x%04X = 0x%02X\n", port, ret));
    }
    return ret;
}

uint16_t Read16(uint16_t port)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    uint16_t ret = 0;
    bool handled = false;
    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            ret = handler->Read16(port);
            if (handler->HasHandled(port))
            {
                handled = true;
                break;
            }
        }
        handler = handler->GetNext();
    }

    if (handled)
    {
        s_CurrentInstance->MarkPort(port);
        s_CurrentInstance->MarkPort(port + 1);
        s_CurrentInstance->Ports[port] = ret;
    }
    else
    {
        s_CurrentInstance->CachePort16(port);
        ret = *((uint16_t*)(&s_CurrentInstance->Ports[port]));
        VERBOSE(printf("Port read 0x%04X = 0x%04X\n", port, ret));
    }
    return ret;
}

void Write8(uint16_t port, uint8_t value)
{
    if (s_Instance0.Allocator == NULL)
        return;

    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    bool handled = false;
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            handler->Write8(port, value);
            if (handled = handler->HasHandled(port))
            {
                break;
            }
        }
        handler = handler->GetNext();
    }
    if (!handled)
    {
        VERBOSE(printf("Port write 0x%04X = 0x%02X\n", port, value));
    }
    s_CurrentInstance->MarkPort(port);
    s_CurrentInstance->Ports[port] = value;
}

void Write16(uint16_t port, uint16_t value)
{
    if (s_Instance0.Allocator == NULL)
        return;

    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    bool handled = false;
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            handler->Write16(port, value);
            if (handled = handler->HasHandled(port))
            {
                break;
            }
        }
        handler = handler->GetNext();
    }
    if (!handled)
    {
        VERBOSE(printf("Port write 0x%04X = 0x%04X\n", port, value));
    }
    s_CurrentInstance->MarkPort(port);
    s_CurrentInstance->MarkPort(port + 1);
    *((uint16_t*)(&s_CurrentInstance->Ports[port])) = value;
}

void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
{
    uint16_t val16 = (uint16_t(valueHi) << 8) | valueLo;
    Write16(port, val16);
}

}

namespace Memory
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count)
    {
        if (s_Instance0.Allocator == NULL)
            return dummy[0];

        VERBOSE(printf("Memory access 0x%04X, size %i\n", offset, size * count));
        return s_CurrentInstance->CacheMemory(offset, size, count);
    }
}

namespace BDA
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count)
    {
        if (s_Instance0.Allocator == NULL)
            return dummy[0];

        VERBOSE(printf("BDA access 0x%04X, size %i\n", 0x400 + offset, size * count));
        return s_CurrentInstance->CacheMemory(0x400 + offset, size, count);
    }
}

}}}
