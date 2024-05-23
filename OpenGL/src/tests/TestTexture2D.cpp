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

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float)); //4 float 

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        IndexBuffer ib(indices, 6);

        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //progjection matrix is in pixel coordinates instead of what was before 
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

        glm::vec4 result = proj * vp;

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

  

        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        //GLCall(glBindVertexArray(0));

        Texture texture("res/textures/Decepticon.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        va.UnBind();
        vb.UnBind();
        ib.UnBind();
        shader.Unbind();
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

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	}


}

