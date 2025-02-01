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

    // ��ָ��·�����ļ�
    std::ifstream stream(filepath);
    // ����ļ��Ƿ�ɹ���
    if (!stream.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        // ���ؿյ���ɫ������ṹ��
        return { "", "" };
    }

    // ����һ��ö�����ͣ���ʾ��ɫ��������
    enum class ShaderType {
        NONE = -1,  // ��ʾδָ����ɫ������
        VERTEX = 0, // ��ʾ������ɫ��
        FRAGMENT = 1 // ��ʾƬ����ɫ��
    };

    std::string line; // ���ڴ洢���ļ��ж�ȡ��ÿһ������
    std::stringstream ss[2]; // �����ַ��������ֱ����ڴ洢�����Ƭ����ɫ���Ĵ���
    ShaderType shaderType = ShaderType::NONE; // ��ʼ��ɫ������Ϊδָ��

    // ���ж�ȡ�ļ�����
    while (getline(stream, line)) {
        // ��鵱ǰ���Ƿ���� "#shader" �ؼ���
        if (line.find("#shader") != std::string::npos) {
            // ����Ƿ�Ϊ������ɫ��
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::VERTEX;
            // ����Ƿ�ΪƬ����ɫ��
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::FRAGMENT;
        }
        else {
            // �����ǰ�в��� "#shader" �����У�������ӵ���Ӧ���ַ�������
            if (shaderType != ShaderType::NONE)
                ss[(int)shaderType] << line << '\n';
        }
    }

    // �ر��ļ�
    stream.close();
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type,const std::string& source) {
   unsigned int id  = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   //��������
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

    //glfw������,�������ȴ���
    glfwMakeContextCurrent(window);

    //��ӡ�汾
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
    //���㻺����
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER,  6 * 2 * sizeof(float), position, GL_STATIC_DRAW);

    //��������
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

        //��0 ȡ3 �� 
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}