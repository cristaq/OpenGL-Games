#include "lab_m1/Tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::Init()
{   
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    carAngle = glm::pi<float>() / 2;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(-1.98834, 0.25009, -1.00143), glm::vec3(1, -0.255824, 0) + glm::vec3(-1.98834, 0.25009, -1.00143), glm::vec3(0, 1, 0));
    carPosition = camera->GetTargetPosition();
    right = 5.f;
    left = -5.f;
    bottom = -5.f;
    top = 5.f;
    fov = 40.f;

    
    minCam = new implemented::Camera();
    minCam->Set(carPosition + glm::vec3(0, 3, 0), carPosition, glm::vec3(1, 0, 0));
    road = new Road();
    tree = new Tree(glm::vec3(3,0,3), 0.2);
    enemies.push_back(new Enemy(0, 0.25, 1));
    enemies.push_back(new Enemy(65, 0.25, 1));
    enemies.push_back(new Enemy(35, 0.75, -1));
    enemies.push_back(new Enemy(90, 0.75, -1));


    {
        Mesh* mesh = new Mesh("car");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "car.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("enemy");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "enemy.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        for (int i = -50; i < 50; i += 1) {
            for (int j = -50; j < 50; j += 1) {
                vertices.push_back(VertexFormat(glm::vec3(i, -0.01, j), glm::vec3(0, 0.5, 0.1)));
                vertices.push_back(VertexFormat(glm::vec3(i, -0.01, j + 1), glm::vec3(0, 0.5, 0.1)));
                vertices.push_back(VertexFormat(glm::vec3(i + 1, -0.01, j + 1), glm::vec3(0, 0.5, 0.1)));
                vertices.push_back(VertexFormat(glm::vec3(i + 1, -0.01, j), glm::vec3(0, 0.5, 0.1)));
            }
        }

        for (int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 2);

            indices.push_back(i + 2);
            indices.push_back(i + 3);
            indices.push_back(i);
        }
        Mesh* mesh = new Mesh("grass");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("Tema");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
    orto= glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);

}


void Tema2::FrameStart()
{
}

void m1::Tema2::RenderScene(float deltaTimeSeconds, glm::mat4 matrix, implemented::Camera* camera)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["grass"], shaders["Tema"], modelMatrix, camera, matrix);
    }

    RenderMesh(road->road, shaders["Tema"], glm::mat4(1), camera, matrix);

    {
        for (Tree* t : road->forest) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, t->position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(t->scale));
            RenderMesh(t->base, shaders["Tema"], modelMatrix, camera, matrix);
            RenderMesh(t->leaves, shaders["Tema"], modelMatrix, camera, matrix);
        }
    }

    {
        for (Enemy* enemy : enemies) {
            glm::mat4 modelMatrix = glm::mat4(1);
            enemy->move(road, deltaTimeSeconds);
            modelMatrix = glm::translate(modelMatrix, enemy->position);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1, 0));
            modelMatrix = glm::rotate(modelMatrix, enemy->angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f));
            RenderMesh(meshes["enemy"], shaders["Tema"], modelMatrix, camera, matrix);
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, carPosition);
        modelMatrix = glm::rotate(modelMatrix, carAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.015f));
        RenderMesh(meshes["car"], shaders["Tema"], modelMatrix, camera, matrix);
    }
}


void Tema2::Update(float deltaTimeSeconds)
{

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene(deltaTimeSeconds, projectionMatrix, camera);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    minCam->position.x = camera->GetTargetPosition().x;
    minCam->position.z = camera->GetTargetPosition().z;
    RenderScene(deltaTimeSeconds, orto, minCam);
    
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera *camera, glm::mat4& projectionMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "car_pos"), 1, glm::value_ptr(carPosition));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 1.7f;
    bool colision = false;
    for (Enemy* enemy : enemies) {
        if (glm::distance(enemy->position, camera->GetTargetPosition()) <= 0.6) {
            colision = true;
        }
    }

    if (window->KeyHold(GLFW_KEY_W) && !colision)
    {
        carPosition.x += deltaTime * cameraSpeed * sin(carAngle);
        carPosition.z += deltaTime * cameraSpeed * cos(carAngle);
        camera->MoveForward(deltaTime * cameraSpeed);
        if (!road->isOnRoad(camera->GetTargetPosition() + glm::normalize(camera->forward) * 0.5f)) {
            carPosition.x -= deltaTime * cameraSpeed * sin(carAngle);
            carPosition.z -= deltaTime * cameraSpeed * cos(carAngle);
            camera->MoveForward(-deltaTime * cameraSpeed);
        }
    }
    if (window->KeyHold(GLFW_KEY_A) && !colision)
    {
        camera->RotateThirdPerson_OY(deltaTime * cameraSpeed);
        carAngle += deltaTime * cameraSpeed;
        if (!road->isOnRoad(camera->GetTargetPosition() - glm::normalize(camera->right) * 0.2f)) {
            camera->RotateThirdPerson_OY(-deltaTime * cameraSpeed);
            carAngle -= deltaTime * cameraSpeed;
        }
    }
    if (window->KeyHold(GLFW_KEY_S) && !colision)
    {
        carPosition.x -= deltaTime * cameraSpeed * sin(carAngle);
        carPosition.z -= deltaTime * cameraSpeed * cos(carAngle);

        camera->MoveForward(-deltaTime * cameraSpeed);
        if (!road->isOnRoad(camera->GetTargetPosition() - glm::normalize(camera->forward) * 0.15f)) {
            camera->MoveForward(deltaTime * cameraSpeed);
            carPosition.x += deltaTime * cameraSpeed * sin(carAngle);
            carPosition.z += deltaTime * cameraSpeed * cos(carAngle);
        }
    }
    if (window->KeyHold(GLFW_KEY_D) && !colision)
    {
        camera->RotateThirdPerson_OY(-deltaTime * cameraSpeed);
        carAngle -= deltaTime * cameraSpeed;
        if (!road->isOnRoad(camera->GetTargetPosition() + glm::normalize(camera->right) * 0.2f)) {
            camera->RotateThirdPerson_OY(deltaTime * cameraSpeed);
            carAngle += deltaTime * cameraSpeed;
        }
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
