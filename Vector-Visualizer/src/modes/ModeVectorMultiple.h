#pragma once

#include "Mode.h"

#include "glad/glad.h"

namespace displayMode {

	class ModeVectorMultiple : public Mode
	{
	public:
		ModeVectorMultiple();
		~ModeVectorMultiple();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

	};

}