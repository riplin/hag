//Copyright 2023-Present riplin

#include <hag/driver.h>

namespace Hag
{

Driver::~Driver()
{

}

void Driver::Dispose()
{
    m_allocator.Destruct(this);
}

}