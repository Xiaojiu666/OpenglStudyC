#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>


struct ShareProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShareProgramSource ParseShader(const std::string& filepath) {

    // 打开指定路径的文件
    std::ifstream stream(filepath);
    // 检查文件是否成功打开
    if (!stream.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        // 返回空的着色器代码结构体
        return { "", "" };
    }

    // 定义一个枚举类型，表示着色器的类型
    enum class ShaderType {
        NONE = -1,  // 表示未指定着色器类型
        VERTEX = 0, // 表示顶点着色器
        FRAGMENT = 1 // 表示片段着色器
    };

    std::string line; // 用于存储从文件中读取的每一行内容
    std::stringstream ss[2]; // 两个字符串流，分别用于存储顶点和片段着色器的代码
    ShaderType shaderType = ShaderType::NONE; // 初始着色器类型为未指定

    // 逐行读取文件内容
    while (getline(stream, line)) {
        // 检查当前行是否包含 "#shader" 关键字
        if (line.find("#shader") != std::string::npos) {
            // 检查是否为顶点着色器
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            // 检查是否为片段着色器
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
        }
        else {
            // 如果当前行不是 "#shader" 声明行，则将其添加到对应的字符串流中
            if (shaderType != ShaderType::NONE)
                ss[(int)shaderType] << line << '\n';
        }
    }

    // 关闭文件
    stream.close();
    return { ss[0].str(), ss[1].str() };
}

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



    float position[] = {
        -0.5f,-0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,

        0.5f, 0.5f,
        -0.5f, 0.5f,
       -0.5f,-0.5f
    };
    


    unsigned int buffer;
    //顶点缓冲区
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER,  6 * 2 * sizeof(float), position, GL_STATIC_DRAW);

    //顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,0);

    //std::string vertextShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0) in vec4 position;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "  gl_Position = position;\n"
    //    "}\n";

    //std::string fragmentShader =
    //    "#version 330 core\n"
    //    "\n"
    //    "layout(location = 0) out vec4 color;\n"
    //    "\n"
    //    "void main()\n"
    //    "{\n"
    //    "  color = vec4(1.0,0.0,0.0,1.0);\n"
    //    "}\n";

    //unsigned int shader = CreatShader(vertextShader, fragmentShader);
    //glUseProgram(shader);

    ShareProgramSource source = ParseShader("res/\shaders/Basic.shader");
    std::cout << source.VertexSource << std::endl;
    std::cout << source.FragmentSource << std::endl;
    unsigned int shader = CreatShader(source.VertexSource, source.FragmentSource);
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
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}