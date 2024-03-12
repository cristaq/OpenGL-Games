#include "tree.h"

Tree::Tree(glm::vec3 position, float scale) : position(position), scale(scale) {
    verticesBase = std::vector<VertexFormat>
    {
        VertexFormat(glm::vec3(-0.5, 0, 0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5, 0, 0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, 2, 0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5, 2, 0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5, 0, -0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5, 0, -0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5, 2, -0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5, 2, -0.5), glm::vec3(0.6, 0.3, 0), glm::vec3(0.2, 0.8, 0.6))
    };

    indicesBase = std::vector<unsigned int>    
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4
    };

    base = new Mesh("base");
    base->InitFromData(verticesBase, indicesBase);

    verticesLeaves = std::vector<VertexFormat>
    {
        VertexFormat(glm::vec3(-1, 1.4, 1), glm::vec3(0.3, 0.4, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, 1.4, 1), glm::vec3(0.3, 0.4, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(1, 1.4, -1), glm::vec3(0.3 ,0.4, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-1, 1.4, -1), glm::vec3(0.3, 0.4, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0, 4, 0), glm::vec3(0.3, 0.4, 0), glm::vec3(0.2, 0.8, 0.6)),
    };

    indicesLeaves = std::vector<unsigned int>
    {
        0, 3, 1,
        1, 3, 2,
        0, 1, 4,
        1, 2, 4,
        3, 2, 4,
        0, 3, 4
    };

    leaves = new Mesh("leaves");
    leaves->InitFromData(verticesLeaves, indicesLeaves);
}

Tree::~Tree() {

}
