#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        return -1;
    }

    // 创建一个窗口和OpenGL上下文
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // 让窗口的上下文成为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // 循环，直到用户关闭窗口
    while (!glfwWindowShouldClose(window)) {
        // 渲染
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix(); // 把当前的坐标系统保存下来

        // 用当前时间计算旋转角度，然后进行旋转
        float time = glfwGetTime();
        float angle = time * 50.0f;         // 每秒钟旋转50度
        glRotatef(angle, 0.0f, 0.0f, 1.0f); // 绕Z轴旋转

        // 绘制三角形
        glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0); // 设置颜色为红色
        glVertex2f(-0.5, -0.5);   // 第一个点的位置
        glVertex2f(0.5, -0.5);    // 第二个点的位置
        glVertex2f(0.0, 0.5);     // 第三个点的位置
        glEnd();

        glPopMatrix(); // 恢复之前保存的坐标系统

        // 交换前后缓冲，更新窗口
        glfwSwapBuffers(window);

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
