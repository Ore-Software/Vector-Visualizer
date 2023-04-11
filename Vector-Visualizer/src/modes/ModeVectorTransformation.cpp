#include "ModeVectorTransformation.h"

namespace displayMode
{
    ModeVectorTransformation::ModeVectorTransformation(std::shared_ptr<std::vector<VectorObject>> vectors, std::shared_ptr<std::vector<float>> vectorBuffer, std::shared_ptr<VertexArray> vectorVA, std::shared_ptr<VertexBuffer> vectorVB)
    {
        m_Vectors = vectors;
        m_VectorBuffer = vectorBuffer;
        m_VectorVA = vectorVA;
        m_VectorVB = vectorVB;

        if (m_Vectors->size() == 1)
        {
            m_Vectors->push_back(m_Vectors->at(0));
            AddVectorBufferData(*m_VectorBuffer, m_Vectors->at(0));
        }

        CalculateMatrix();
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
        ImGui::Text("Matrix entries");
        ImGui::SliderFloat3("Translate", &m_Translate.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation Axis", &m_RotateAxis.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Rotation Angle", &m_RotateAngle, 0.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &m_Scale.x, -10.0f, 10.0f);

        ImGui::Text("Transform Matrix");
        for (int i = 0; i < 4; i++)
        {
            ImGui::Text("%f %f %f %f", m_Transform[0][i], m_Transform[1][i], m_Transform[2][i], m_Transform[3][i]); // transposed
        }

        if (ImGui::Button("Update Matrix"))
        {
            CalculateMatrix();
            UpdateTransformedVector();
        }

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Initial Vector");
        ImGui::SliderFloat3("Origin", &m_Vectors->at(0).m_Origin.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Direction", &m_Vectors->at(0).m_Direction.x, -10.0f, 10.0f);
        ImGui::ColorEdit4("Color", &m_Vectors->at(0).m_Color.x);
        if (ImGui::Button("Update Initial Vector"))
        {
            EditVectorBufferData(*m_VectorBuffer, *m_Vectors, 0);
            UpdateTransformedVector();

            m_VectorVA->Bind();
            m_VectorVB->Bind();
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
        };

        ImGui::Spacing();

        ImGui::Text("Transformed Vector");
        ImGui::Text("Origin: %f %f %f", m_Vectors->at(1).m_Origin.x, m_Vectors->at(1).m_Origin.y, m_Vectors->at(1).m_Origin.z);
        ImGui::Text("Direction: %f %f %f", m_Vectors->at(1).m_Direction.x, m_Vectors->at(1).m_Direction.y, m_Vectors->at(1).m_Direction.z);

        ImGui::End();
    }

    void ModeVectorTransformation::CalculateMatrix()
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, m_Scale);
        transform = glm::rotate(transform, glm::radians(m_RotateAngle), m_RotateAxis);
        transform = glm::translate(transform, m_Translate);

        m_Transform = transform;
    }

    void ModeVectorTransformation::UpdateTransformedVector()
    {
        VectorObject transformedVector;
        transformedVector.m_Origin = glm::vec4(m_Vectors->at(0).m_Origin, 1.0f);
        transformedVector.m_Direction = m_Transform * glm::vec4(m_Vectors->at(0).m_Direction, 1.0f);

        m_Vectors->at(1) = transformedVector;
        EditVectorBufferData(*m_VectorBuffer, *m_Vectors, 1);

        m_VectorVA->Bind();
        m_VectorVB->Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_VectorBuffer->size(), m_VectorBuffer->data(), GL_DYNAMIC_DRAW);
    }

}