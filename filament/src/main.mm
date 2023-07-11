
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <cstdlib>
#include <filamat/MaterialBuilder.h>
#include <filament/Camera.h>
#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/RenderableManager.h>
#include <filament/Renderer.h>
#include <filament/Scene.h>
#include <filament/SwapChain.h>
#include <filament/VertexBuffer.h>
#include <filament/View.h>
#include <iostream>
#include <math/vec3.h>
#include <math/vec4.h>
#include <ostream>
#include <utils/EntityManager.h>
#include <utils/JobSystem.h>

using namespace filament;
using namespace filament::math;
using namespace utils;
using namespace filamat;

// 这些是我们将绘制的三角形的顶点坐标
static constexpr float TRIANGLE_VERTICES[3 * 3] = {
    -0.5f,
    -0.5f,
    0.0f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    0.5f,
    0.0f,
};

extern "C" {
void* get_nsview(GLFWwindow* window) {
    NSWindow* nswin = (NSWindow*)glfwGetCocoaWindow(window);
    NSView* nsview = [nswin contentView];
    [nsview setWantsBestResolutionOpenGLSurface:YES];
    return (void*)nsview;
}
}

int main() {
    // 初始化一个窗口
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Filament Triangle Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create window." << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 初始化GLEW
    if (glewInit() != GLEW_OK) {
        return -1;
    }
    // 创建引擎
    Engine* engine = Engine::create(filament::backend::Backend::OPENGL);
    if (engine == nullptr) {
        std::cerr << "Failed to create engine." << std::endl;
        return -1;
    }
    void* nativeWindow = get_nsview(window);
    if (nativeWindow == nullptr) {
        std::cerr << "Failed to get native window." << std::endl;
        return -1;
    }

    SwapChain* swapChain = engine->createSwapChain(nativeWindow);
    Renderer* renderer = engine->createRenderer();
    Camera* camera = engine->createCamera(utils::EntityManager::get().create());
    View* view = engine->createView();
    Scene* scene = engine->createScene();

    view->setCamera(camera);
    view->setScene(scene);

    VertexBuffer* vbo = VertexBuffer::Builder()
                            .vertexCount(3)
                            .bufferCount(1)
                            .attribute(VertexAttribute::POSITION,
                                       0,
                                       VertexBuffer::AttributeType::FLOAT3)
                            .build(*engine);
    vbo->setBufferAt(*engine,
                     0,
                     VertexBuffer::BufferDescriptor(TRIANGLE_VERTICES,
                                                    sizeof(TRIANGLE_VERTICES)));

    IndexBuffer* ibo = IndexBuffer::Builder()
                           .indexCount(3)
                           .bufferType(IndexBuffer::IndexType::UINT)
                           .build(*engine);
    uint32_t indices[3] = {0, 1, 2};
    ibo->setBuffer(*engine,
                   IndexBuffer::BufferDescriptor(indices, sizeof(indices)));

    // 创建一个材质实例，并将纯色材质设置给它

    // 创建渲染对象
    Entity renderable = EntityManager::get().create();
    filament::RenderableManager::Builder(1)
        .boundingBox({{-1, -1, -1}, {1, 1, 1}})
        .geometry(0, RenderableManager::PrimitiveType::TRIANGLES, vbo, ibo)
        .build(*engine, renderable);

    scene->addEntity(renderable);

    camera->lookAt(
        math::float3(0, 0, 1), math::float3(0, 0, 0), math::float3(0, 1, 0));
    camera->setProjection(
        45, 1.5f, 0.1f, 100.0f); // aspectRatio 是你的窗口宽度除以高度

    // 主循环
    while (!glfwWindowShouldClose(window)) {

        if (renderer->beginFrame(swapChain)) {
            // 渲染视图
            renderer->render(view);

            // 结束帧
            renderer->endFrame();
        }

        // 更新窗口事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    engine->destroy(renderable);
    engine->destroy(scene);
    engine->destroy(view);
    engine->destroy(renderer);
    engine->destroy(vbo);
    engine->destroy(ibo);
    Engine::destroy(&engine);

    // 销毁GLFW窗口
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
