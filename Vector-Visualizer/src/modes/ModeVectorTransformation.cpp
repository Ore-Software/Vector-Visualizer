#include "ModeVectorTransformation.h"

namespace displayMode
{
    ModeVectorTransformation::ModeVectorTransformation(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB, const VertexBufferLayout& vertexLayout)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;

        m_Translate = glm::vec3(0.0f);
        m_RotateAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        m_RotateAngle = 0.0f;
        m_Scale = glm::vec3(1.0f);

        CalculateMatrix();

        m_TriangleBuffer = std::make_shared<std::vector<float>>();
        m_TriangleVB = std::make_shared<VertexBuffer>(m_TriangleBuffer->data(), sizeof(float) * m_TriangleBuffer->size(), MODE::DYNAMIC);
        m_TriangleVA = std::make_shared<VertexArray>();
        glGenBuffers(1, &m_TriangleIB);
        m_Indices =
        {
            0, 1, 4,
            4, 2, 0,
            1, 5, 7,
            7, 4, 1,
            0, 3, 6,
            6, 2, 0,
            0, 1, 5,
            5, 3, 0,
            3, 5, 7,
            7, 6, 3,
            2, 4, 7,
            7, 6, 2
        };

        m_TriangleVertices = {};
        m_TriangleVertices.push_back(glm::vec3(0.0f));
        m_TriangleVertices.push_back(glm::vec3(m_Transform[0][0], m_Transform[0][1], m_Transform[0][2]));
        m_TriangleVertices.push_back(glm::vec3(m_Transform[1][0], m_Transform[1][1], m_Transform[1][2]));
        m_TriangleVertices.push_back(glm::vec3(m_Transform[2][0], m_Transform[2][1], m_Transform[2][2]));
        m_TriangleVertices.push_back(glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[2]));
        m_TriangleVertices.push_back(glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[3]));
        m_TriangleVertices.push_back(glm::vec3(m_TriangleVertices[2] + m_TriangleVertices[3]));
        m_TriangleVertices.push_back(glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[2] + m_TriangleVertices[3]));
       
        m_TriangleColors = {};
        m_TriangleColors.push_back(glm::vec4(0.2f, 0.2f, 0.2f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.5f));
        m_TriangleColors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

        m_VertexLayout = vertexLayout;

        // when it is initialized it will have 1 vector, and we will add a deep copy of it and transform the copy
        m_Vectors->push_back(m_Vectors->at(0));
        AddVectorBufferLineData(*m_VectorBuffer, m_Vectors->at(0));

        for (int i = 0; i < m_TriangleVertices.size(); i++)
        {
            AddVectorBufferTriangleData(*m_TriangleBuffer, m_TriangleVertices.at(i), m_TriangleColors.at(i));
        }
    }

    ModeVectorTransformation::~ModeVectorTransformation()
    {
    }

    void ModeVectorTransformation::OnUpdate(float deltaTime)
    {
    }

    void ModeVectorTransformation::OnRender()
    {
    }

    void ModeVectorTransformation::OnImGuiRender()
    {
        // imgui matrix controls
        ImGui::Begin("Matrix Transformation");
        ImGui::Text("Rotate and scale direction:");
        ImGui::SliderFloat3("Rotation Axis", &m_RotateAxis.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Rotation Angle", &m_RotateAngle, 0.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &m_Scale.x, -10.0f, 10.0f);

        ImGui::Text("Transform Matrix");
        for (int i = 0; i < 3; i++)
        {
            ImGui::Text("%f %f %f", m_Transform[0][i], m_Transform[1][i], m_Transform[2][i]); // transposed
        }

        ImGui::Spacing();

        if (ImGui::Button("Show/Hide Matrix"))
            m_ShowParallelepiped = !m_ShowParallelepiped;
        
        ImGui::End();

        ImGui::Begin("Vectors");
        ImGui::Text("Initial Vector");
        ImGui::SliderFloat3("Origin", &m_Vectors->at(0).m_Origin.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Direction", &m_Vectors->at(0).m_Direction.x, -10.0f, 10.0f);
        ImGui::ColorEdit4("Color", &m_Vectors->at(0).m_Color.x);
        
        EditVectorBufferLineData(*m_VectorBuffer, *m_Vectors, 0);
        UpdateTransformedVector();
        ImGui::Spacing();

        ImGui::Text("Translate origin:");
        ImGui::SliderFloat3("", &m_Translate.x, -10.0f, 10.0f);
        ImGui::Spacing();

        ImGui::Text("Transformed Vector");
        ImGui::Text("Origin:    %f %f %f", m_Vectors->at(1).m_Origin.x, m_Vectors->at(1).m_Origin.y, m_Vectors->at(1).m_Origin.z);
        ImGui::Text("Direction: %f %f %f", m_Vectors->at(1).m_Direction.x, m_Vectors->at(1).m_Direction.y, m_Vectors->at(1).m_Direction.z);

        if (m_ShowParallelepiped)
        {
            m_TriangleVA->Bind();
            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        }

        ImGui::End();

        // redraw vectors
        CalculateMatrix();
        UpdateTransformedVector();
        CalcParallelepipedVertices();
        Redraw();
    }

    void ModeVectorTransformation::CalculateMatrix()
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, m_Scale);
        transform = glm::rotate(transform, glm::radians(m_RotateAngle), m_RotateAxis);

        m_Transform = transform;
    }

    void ModeVectorTransformation::UpdateTransformedVector()
    {
        VectorObject transformedVector;
        transformedVector.m_Origin = glm::vec4(m_Vectors->at(0).m_Origin + m_Translate, 1.0f);
        transformedVector.m_Direction = m_Transform * glm::vec4(m_Vectors->at(0).m_Direction, 1.0f);

        m_Vectors->at(1) = transformedVector;
        EditVectorBufferLineData(*m_VectorBuffer, *m_Vectors, 1);
    }

    void ModeVectorTransformation::CalcParallelepipedVertices()
    {      
        m_TriangleVertices[0] = (glm::vec3(0.0f));
        m_TriangleVertices[1] = (glm::vec3(m_Transform[0][0], m_Transform[0][1], m_Transform[0][2]));
        m_TriangleVertices[2] = (glm::vec3(m_Transform[1][0], m_Transform[1][1], m_Transform[1][2]));
        m_TriangleVertices[3] = (glm::vec3(m_Transform[2][0], m_Transform[2][1], m_Transform[2][2]));
        m_TriangleVertices[4] = (glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[2]));
        m_TriangleVertices[5] = (glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[3]));
        m_TriangleVertices[6] = (glm::vec3(m_TriangleVertices[2] + m_TriangleVertices[3]));
        m_TriangleVertices[7] = (glm::vec3(m_TriangleVertices[1] + m_TriangleVertices[2] + m_TriangleVertices[3]));
        
        for (int i = 0; i < 8; i++)
        {
            EditVectorBufferTriangleData(*m_TriangleBuffer, m_TriangleVertices[i], m_TriangleColors[i], i);
        }

        m_TriangleVA->Bind();
        m_TriangleVB->Bind();
        glBufferData(GL_ARRAY_BUFFER, m_TriangleBuffer->size() * sizeof(unsigned int), m_TriangleBuffer->data(), GL_DYNAMIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TriangleIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_DYNAMIC_DRAW);
        m_TriangleVA->AddBuffer(*m_TriangleVB, m_VertexLayout);
    }
}