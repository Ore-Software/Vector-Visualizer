#include "ModeVectorTransformation.h"

#include "../external/glm/glm.hpp"
#include "../external/glm/gtc/matrix_transform.hpp"

namespace displayMode
{
    ModeVectorTransformation::ModeVectorTransformation()
    {
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
        //ImGui::SliderFloat3("Row 1", &transMatrix[0], -10.0f, 10.0f);
        //ImGui::SliderFloat3("Row 2", &transMatrix[3], -10.0f, 10.0f);
        //ImGui::SliderFloat3("Row 3", &transMatrix[6], -10.0f, 10.0f);
        ImGui::End();
    }

}