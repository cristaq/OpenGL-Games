#pragma once
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/lab4//transform3D.h"
#include "road.h"
#include "enemy.h"
#include "tree.h"

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderScene(float deltaTimeSeconds, glm::mat4 matrix, implemented::Camera* camera);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera* camera, glm::mat4& projectionMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 orto;

        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;

        Road* road;
        Tree* tree;
        std::vector<Enemy*> enemies;
        float carAngle = glm::pi<float>() / 2;

        ViewportArea miniViewportArea;
        implemented::Camera* minCam;

        glm::vec3 carPosition;

    };
}   // namespace m1
