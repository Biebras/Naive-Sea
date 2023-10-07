#pragma once

#include "model.h"

class ModelCreator 
{
    public:
        static Model* CreatePlaneModel(float width, float height, int quadCount)
        {
            Model* model = new Model();

            float quadWidth = width / quadCount;
            float quadHeight = height / quadCount;

            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<glm::vec3> normals;

            for (int i = 0; i < quadCount; i++) {
                for (int j = 0; j < quadCount; j++) {
                    float x = i * quadWidth;
                    float y = j * quadHeight;

                    // Create vertices for quad
                    Vertex v1 = {glm::vec3(x, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
                    Vertex v2 = {glm::vec3(x + quadWidth, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)};
                    Vertex v3 = {glm::vec3(x + quadWidth, 0.0f, y + quadHeight), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
                    Vertex v4 = {glm::vec3(x, 0.0f, y + quadHeight), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)};

                    // Add vertices to vector
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                    vertices.push_back(v3);
                    vertices.push_back(v4);

                    // Add indices to vector
                    int baseIndex = i * quadCount * 4 + j * 4;
                    indices.push_back(baseIndex);
                    indices.push_back(baseIndex + 1);
                    indices.push_back(baseIndex + 2);
                    indices.push_back(baseIndex + 2);
                    indices.push_back(baseIndex + 3);
                    indices.push_back(baseIndex);

                    // Add normals to vector
                    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                }
            }

            // Add triangles to model
            model->AddTriangles(vertices, indices, normals);

            return model;
        }
};
