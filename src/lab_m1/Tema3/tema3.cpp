#include "lab_m1/tema3/tema3.h"


#include <vector>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");
    srand(time(0));
    camera = new implemented::Camera();
    camera->Set(slopeCenter + glm::vec3(0, 5, 5), slopeCenter + glm::vec3(0, -1.5, 0), glm::vec3(0, 0.5, -0.3));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
    
    

    // Load textures

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "leaf.jpg").c_str(), GL_REPEAT);
        mapTextures["leaf"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.png").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.jpg").c_str(), GL_REPEAT);
        mapTextures["stone"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "iron.jpg").c_str(), GL_REPEAT);
        mapTextures["iron"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ski.jpg").c_str(), GL_REPEAT);
        mapTextures["ski"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "skin.jpg").c_str(), GL_REPEAT);
        mapTextures["skin"] = texture;
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(slopeSize, 0.0f, slopeSize),    // top right
            glm::vec3(slopeSize, 0.0f, -slopeSize),    // bottom right
            glm::vec3(-slopeSize, 0.0f, -slopeSize),    // bottom left
            glm::vec3(-slopeSize, 0.0f, slopeSize),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            // TODO(student): Complete texture coordinates for the square
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.f, 1.f),
            glm::vec2(1.f, 0.f)

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    camera->Set(slopeCenter + glm::vec3(0, 5, 5), slopeCenter + glm::vec3(0, -1.5, 0), glm::vec3(0, 0.5, -0.3));
    if (!lost) {
        numberOfLights = 0;

        if (timeElapsed >= 0.4) {
            objects.push_back(new Object(slopeSize, slopeCenter));
            timeElapsed = 0;
        }
        else {
            timeElapsed += deltaTimeSeconds;
        }

        for (Object* o : objects) {
            if (o->getType() == 0 || o->getType() == 1) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
                lights[numberOfLights] = (modelMatrix * glm::vec4(o->getPos(), 0));
                types[numberOfLights] = o->getType();
                numberOfLights++;
            }
            else if (o->getType() == 3) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
                lights[numberOfLights] = (modelMatrix * glm::vec4(o->getPos(), 0));
                types[numberOfLights] = o->getType();
                numberOfLights++;

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
                lights[numberOfLights] = (modelMatrix * glm::vec4(o->getPos(), 0));
                types[numberOfLights] = o->getType() + 1;
                numberOfLights++;
            }
        }

        vector<Object*>::iterator o;
        for (o = objects.begin(); o != objects.end();) {
            (*o)->move(deltaTimeSeconds, playerAngle);
            switch ((*o)->getType())
            {
            case 0:
                RenderGift((*o)->getPos());
                break;

            case 1:
                RenderTree((*o)->getPos());
                break;

            case 2:
                RenderStone((*o)->getPos());
                break;

            case 3:
                RenderPole((*o)->getPos());
                break;

            default:
                break;
            }

            if ((*o)->collision(slopeCenter)) {
                if ((*o)->getType() == 0) {
                    score++;
                }
                else {
                    lost = 1;
                }
                delete(*o);
                o = objects.erase(o);
            } else if ((*o)->getPos().z < -slopeSize || 
                    (*o)->getPos().x > slopeSize || (*o)->getPos().x < - slopeSize) {
                delete(*o);
                o = objects.erase(o);
            }
            else {
                o++;
            }
        }

    } else if (lost == 1) {
        for (Object* o : objects) {
            delete(o);
        }
        numberOfLights = 0;
        objects.clear();
        cout << "Scor: " << score << endl;
        score = 0;
        lost = 2;
    }
    RenderPlayerSlope(glm::vec3(0));
}


void Tema3::FrameEnd()
{
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glm::vec3 eyePosition = camera->GetPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    GLuint location = glGetUniformLocation(shader->program, "light_pos");
    glUniform3fv(location, numberOfLights, glm::value_ptr(lights[0]));

    location = glGetUniformLocation(shader->program, "type");
    glUniform1iv(location, numberOfLights, types);

    int number = glGetUniformLocation(shader->program, "numberOfLights");
    glUniform1i(number, numberOfLights);

    int cutoff = glGetUniformLocation(shader->program, "cutoff_angle");
    glUniform1f(cutoff, glm::radians(cutOffAngle));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (mesh == meshes["square"]) {
        glUniform1f(glGetUniformLocation(shader->program, "time"), Engine::GetElapsedTime());
    }
    else {
        glUniform1f(glGetUniformLocation(shader->program, "time"), 0);
    }

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W))
        {
            // Translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_A))
        {
            // Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            // Translate the camera backwards
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_D))
        {
            // Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_Q))
        {
            // Translate the camera down
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_E))
        {
            // Translate the camera up
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE && lost == 2) {
        lost = 0;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
        camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
    }

    
    
    glm::vec2 resolution = window->GetResolution();
    playerAngle = RADIANS(80) * (mouseX - resolution.x / 2) / (resolution.x / 2);

}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}

void m1::Tema3::RenderTree(glm::vec3 pos)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.7, 0.1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["wood"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, 0));
    RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["leaf"]);
}

void m1::Tema3::RenderStone(glm::vec3 pos)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, 0.5, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["stone"]);
}

void m1::Tema3::RenderPole(glm::vec3 pos)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 1, 0.1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["iron"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.7, 0.1));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["iron"]);
}

void m1::Tema3::RenderGift(glm::vec3 pos)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["gift"]);
}

void m1::Tema3::RenderPlayerSlope(glm::vec3 pos)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, slopeCenter);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.03, 1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5, 0.5, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, slopeCenter);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.03, 1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5, 0.5, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["ski"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, slopeCenter);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, playerAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.5, 0.3));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.6, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["skin"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::translate(modelMatrix, slopeCenter);
    RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["snow"]);
}
