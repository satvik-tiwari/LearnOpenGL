#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	
	TestTexture2D::TestTexture2D()	
        : m_TranslationA(200, 200, 0), m_TranslationB(500, 300, 0)
	{

		float positions[] = {
		   -50.0f, -50.0f, 0.0f, 0.0f, //0 bottom left texture coordinate     //1 unit = 1 pixel
			50.0f, -50.0f, 1.0f, 0.0f, //1 bottom right texture coordinate
			50.0f,  50.0f, 1.0f, 1.0f,   //2 top right texture coordinate
		   -50.0f,  50.0f, 0.0f, 1.0f    //3 top left texture coordinate
		};

		unsigned int indices[] = {
		   0, 1, 2,
		   2, 3, 0
		};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_VAO = std::make_unique<VertexArray>();

        //VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float)); //4 float 

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(vb, layout);

        m_IB = std::make_unique<IndexBuffer>(indices, 6);

        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //progjection matrix is in pixel coordinates instead of what was before 
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader->Bind();

        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/Decepticon.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IB, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        auto io = ImGui::GetIO();
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	}


}

