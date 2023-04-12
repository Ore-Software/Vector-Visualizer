#pragma once

#include <vector>
#include "VectorObject.h"

inline void AddVectorBufferLineData(std::vector<float>& buffer, VectorObject vectorObject)
{
    buffer.push_back(vectorObject.m_Origin.x);
    buffer.push_back(vectorObject.m_Origin.y);
    buffer.push_back(vectorObject.m_Origin.z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);

    buffer.push_back(vectorObject.GetEndPoint().x);
    buffer.push_back(vectorObject.GetEndPoint().y);
    buffer.push_back(vectorObject.GetEndPoint().z);

    buffer.push_back(vectorObject.m_Color.x);
    buffer.push_back(vectorObject.m_Color.y);
    buffer.push_back(vectorObject.m_Color.z);
    buffer.push_back(vectorObject.m_Color.w);
}

inline void EditVectorBufferLineData(std::vector<float>& buffer, std::vector<VectorObject>& vectorObjects, unsigned int index)
{
    buffer[14 * index + 0] = vectorObjects[index].m_Origin.x;
    buffer[14 * index + 1] = vectorObjects[index].m_Origin.y;
    buffer[14 * index + 2] = vectorObjects[index].m_Origin.z;

    buffer[14 * index + 3] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 4] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 5] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 6] = vectorObjects[index].m_Color.w;

    buffer[14 * index + 7] = vectorObjects[index].GetEndPoint().x;
    buffer[14 * index + 8] = vectorObjects[index].GetEndPoint().y;
    buffer[14 * index + 9] = vectorObjects[index].GetEndPoint().z;

    buffer[14 * index + 10] = vectorObjects[index].m_Color.x;
    buffer[14 * index + 11] = vectorObjects[index].m_Color.y;
    buffer[14 * index + 12] = vectorObjects[index].m_Color.z;
    buffer[14 * index + 13] = vectorObjects[index].m_Color.w;
}

inline void AddVectorBufferTriangleData(std::vector<float>& buffer, glm::vec3 vertexPositions, glm::vec4 vertexColors)
{
    buffer.push_back(vertexPositions.x);
    buffer.push_back(vertexPositions.y);
    buffer.push_back(vertexPositions.z);

    buffer.push_back(vertexColors.r);
    buffer.push_back(vertexColors.g);
    buffer.push_back(vertexColors.b);
    buffer.push_back(vertexColors.a);
}

inline void EditVectorBufferTriangleData(std::vector<float>& buffer, glm::vec3 vertexPositions, glm::vec4 vertexColors, unsigned int index)
{
    buffer[7 * index + 0] = vertexPositions.x;
    buffer[7 * index + 1] = vertexPositions.y;
    buffer[7 * index + 2] = vertexPositions.z;
           
    buffer[7 * index + 3] = vertexColors.x;
    buffer[7 * index + 4] = vertexColors.y;
    buffer[7 * index + 5] = vertexColors.z;
    buffer[7 * index + 6] = vertexColors.w;
}