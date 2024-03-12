#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/lab3/object2D.h"
#include "Duck.h"
#include "Hud.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    duck = new Duck();
    duck->loadMeshes(meshes);
    hud = new Hud(resolution);
    hud->loadMeshes();

}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.7, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    modelMatrix = duck->movement(deltaTimeSeconds, resolution, escaped, dead);
    
    RenderMesh(hud->grass, shaders["VertexColor"], glm::vec3(0, 0, 0), glm::vec3(1));
    RenderMesh2D(duck->head, shaders["VertexColor"], modelMatrix);
    RenderMesh2D(duck->body, shaders["VertexColor"], modelMatrix);
    RenderMesh2D(duck->beak, shaders["VertexColor"], modelMatrix);

    if (!dead) {
        modelMatrix = duck->flapWing(deltaTimeSeconds);
    }
    RenderMesh2D(duck->wings, shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < lives; i++) {
        RenderMesh(hud->heart, shaders["VertexColor"], glm::vec3(i * 50, 0, 0), glm::vec3(1));
    }

    for (int i = 0; i < score; i++) {
    RenderMesh(hud->score, shaders["VertexColor"], glm::vec3(i * 200 / 25, 0, 0), glm::vec3(1));
    }

    for (int i = 0; i < numOfBullets; i++) {
        RenderMesh(hud->bullet, shaders["VertexColor"], glm::vec3(i * 30, 0, 0), glm::vec3(1));
    }

    RenderMesh(hud->scoreBar, shaders["VertexColor"], glm::vec3(0, 0, 0), glm::vec3(1));

    if (numOfBullets == 0 || duck->getTime() > 5) {
        lives--;
        duck->escapeDuck();
        numOfBullets = 3;
    }

    if ((score == 25 && !dead) || (lives == 0 && !escaped)) {
        lives = 3;
        numOfBullets = 3;
        duck->resetSpeed();
        score = 0;
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (!escaped && !dead && button == 1 && numOfBullets > 0) {
        if (duck->checkColissionBullet(mouseX, resolution.y - mouseY)) {
            duck->killDuck();
            score++;
            if (score % 5 == 0 && score != 0) {
                duck->incrementSpeed();
            }
            numOfBullets = 3;
        }
        else {
            numOfBullets--;
        }
    }

    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
