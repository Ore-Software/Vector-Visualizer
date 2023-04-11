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

    //glm::vec3 endPoint = vectorObject.GetEndPoint();

    buffer.push_back(vectorObject.GetEndPoint().x);
    buffer.push_back(vectorObject.GetEndPoint().y);
    buffer.push_back(vectorObject.GetEndPoint().z);

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

    //glm::vec3 endPoint = vectorObjects[index].GetEndPoint();

    buffer[14 * index + 7] = vectorObjects[index].m_Origin.x + vectorObjects[index].m_Direction.x;
    buffer[14 * index + 8] = vectorObjects[index].m_Origin.y + vectorObjects[index].m_Direction.y;
    buffer[14 * index + 9] = vectorObjects[index].m_Origin.z + vectorObjects[index].m_Direction.z;

    buffer[14 * index + 10] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 11] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 12] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 13] = vectorObjects[index].m_Color.w;
}