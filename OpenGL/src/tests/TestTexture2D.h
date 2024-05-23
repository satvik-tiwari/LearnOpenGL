#pragma once

#include "Test.h"

namespace test {

	class TestTexture2D : public Test
	{
	private:
		

	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};

}
