#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>


#include "components/simple_scene.h"
#include "components/transform.h"
#include "Object.h"
#include "lab_m1/lab5/lab_camera.h"

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderTree(glm::vec3 pos);
        void RenderStone(glm::vec3 pos);
        void RenderPole(glm::vec3 pos);
        void RenderGift(glm::vec3 pos);
        void RenderPlayerSlope(glm::vec3 pos);

        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::vector<Object*> objects;
        float slopeSize = 10;
        glm::vec3 slopeCenter = glm::vec3(0, 0, 0);
        float playerAngle = 0;
        float timeElapsed = 0.5;
        int score = 0;
        int lost = 0;

        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::vec3 lights[100];
        int types[100];
        int numberOfLights = 0;
        float cutOffAngle = 45;

        float materialShininess = 30;
        float materialKd = 0.5;
        float materialKs = 0.5;

    };
}   // namespace m1
