#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

static unsigned int CompileShader(unsigned int type,const std::string& source) {
   unsigned int id  = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   //处理问题
   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) {
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char* message = (char*)alloca(length * sizeof(char));
       glGetShaderInfoLog(id, length, &length, message);
       std::cout << message << std::endl;
   }

   return id;
}

static int CreatShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program =  glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

  

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //glfw上下文,必须最先创建
    glfwMakeContextCurrent(window);

    //打印版本
    std::cout << glGetString(GL_VERSION) << std::endl; 


    if (glewInit() == GLEW_OK)
        std::cout << "Sucessful" << std::endl;



    float position[6] = {
        -0.5f,-0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    //顶点缓冲区
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

    //顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,0);

    std::string vertextShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";

    unsigned int shader = CreatShader(vertextShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // old gl
   /*     glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.0f,0.5f);
        glVertex2f(0.5f,-0.5f);
        glEnd();*/

        //从0 取3 个 
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}