#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    //window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /*unsigned int a;
    glGenBuffers(1, &a);*/

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

       

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float)); //4 float 
       
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

       // GLCall(glEnableVertexAttribArray(0));
       // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        /*unsigned int ibo;
        GLCall(glGenBuffers(1, &ibo));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indices, GL_STATIC_DRAW));*/

        IndexBuffer ib(indices, 6);

        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //progjection matrix is in pixel coordinates instead of what was before 
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        
        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        
        glm::vec4 result = proj * vp;
        
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        //ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        
        /*std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMEMT" << std::endl;
        std::cout << source.FragmentSource << std::endl;*/

        //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        //GLCall(glUseProgram(shader));

        /*GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));*/

        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        //GLCall(glBindVertexArray(0));

        Texture texture("res/textures/Decepticon.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
       
        
        va.UnBind();
        vb.UnBind();
        ib.UnBind();
        shader.Unbind();

        /*GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
       
        
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        test::Test* current;
        test::TestClearColor test;

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(500, 300, 0);

        float r = 0.0f;
        float increment = 0.05f;

        /*bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);*/

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //GLCall(glClear(GL_COLOR_BUFFER_BIT));

            //GLClearError();
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;

                shader.Bind();
                //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;

                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            //GLCall(glUseProgram(shader));
            //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            //GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
           // GLCall(glBindVertexArray(vao));
            //va.Bind();
            //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
          //  ib.Bind();

            /*GLCall(glEnableVertexAttribArray(0));
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/

            //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            //glDrawArrays(GL_TRIANGLES, 0, 6);

           

            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r = r + increment;

            {
                //static float f = 0.0f;
                //static int counter = 0;

                //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                //ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                //    counter++;
                //ImGui::SameLine();
                //ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                //ImGui::End();
            }

            //ASSERT(GLLogCall());

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }


    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    //std::cin.get(;
    return 0;
}