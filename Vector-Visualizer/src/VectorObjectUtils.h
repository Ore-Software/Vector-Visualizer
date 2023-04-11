#pragma once

#include <vector>
#include "VectorObject.h"

inline void AddVectorBufferData(std::vector<float>& buffer, VectorObject vectorObject)
{
    buffer.push_back(vectorObject.m_Origin.x);
    buffer.push_back(vectorObject.m_Origin.y);
    buffer.push_back(vectorObject.m_Origin.z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);

    buffer.push_back(vectorObject.m_EndPoint.x);
    buffer.push_back(vectorObject.m_EndPoint.y);
    buffer.push_back(vectorObject.m_EndPoint.z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);
}

inline void EditVectorBufferData(std::vector<float>& buffer, const std::vector<VectorObject>& vectorObjects, unsigned int index)
{
    buffer[14 * index + 0] = vectorObjects[index].m_Origin.x;
    buffer[14 * index + 1] = vectorObjects[index].m_Origin.y;
    buffer[14 * index + 2] = vectorObjects[index].m_Origin.z;

    buffer[14 * index + 3] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 4] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 5] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 6] = vectorObjects[index].m_Color.w;

    buffer[14 * index + 7] = vectorObjects[index].m_EndPoint.x;
    buffer[14 * index + 8] = vectorObjects[index].m_EndPoint.y;
    buffer[14 * index + 9] = vectorObjects[index].m_EndPoint.z;

    buffer[14 * index + 10] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 11] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 12] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 13] = vectorObjects[index].m_Color.w;
}