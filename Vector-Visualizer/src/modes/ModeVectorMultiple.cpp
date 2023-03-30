#include "ModeVectorMultiple.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

namespace displayMode 
{
    ModeVectorMultiple::ModeVectorMultiple()
    {
    }

    ModeVectorMultiple::~ModeVectorMultiple()
    {
    }

    void ModeVectorMultiple::OnUpdate(float deltaTime)
    {
    }

    void ModeVectorMultiple::OnRender()
    {
    }

    void ModeVectorMultiple::OnImGuiRender()
    {
        // imgui vector controls
        ImGui::Begin("Multiple Vectors Controls");
        //for (int j = 0; j < vectors.size(); j++)
        //{
        //    ImGui::PushID(j);
        //    ImGui::Text("Vector %d", j + 1);
        //    ImGui::SliderFloat3("Origin", &vectors[j].m_Origin.x, -10.0f, 10.0f);
        //    ImGui::SliderFloat3("Direction", &vectors[j].m_Direction.x, -10.0f, 10.0f);
        //    ImGui::ColorEdit4("Color", &vectors[j].m_Color.x);
        //    if (ImGui::Button("Apply Changes"))
        //    {
        //        EditVectorBufferData(vectorBuffer, vectors, j);

        //        vectorVA.Bind();
        //        vectorVB.Bind();
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
        //    };

        //    if (ImGui::Button("Remove vector"))
        //    {
        //        vectors.erase(vectors.begin() + j);
        //        
        //        // reconstruct buffer of vector vertices
        //        vectorBuffer.clear();
        //        for (VectorObject vec : vectors)
        //        {
        //            AddVectorBufferData(vectorBuffer, vec);
        //        }

        //        vectorVA.Bind();
        //        vectorVB.Bind();
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
        //    }

        //    ImGui::Spacing();
        //    ImGui::Separator();
        //    ImGui::PopID();
        //}
        if (ImGui::Button("Add vector"))
        {
            //VectorObject newVec(glm::vec3(0.0f, 0.0f, 0.0f), glm::ballRand(5.0f), glm::vec4(glm::abs(glm::ballRand(1.0f)), 1.0f)); // uses random vector direction and random color
            //vectors.push_back(newVec);
            //AddVectorBufferData(vectorBuffer, newVec);

            //vectorVA.Bind();
            //vectorVB.Bind();
            //glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vectorBuffer.size(), vectorBuffer.data(), GL_DYNAMIC_DRAW);
        }
        ImGui::End();
    }
}