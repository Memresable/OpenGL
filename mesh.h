/* date = July 5th 2022 4:15 pm */

#ifndef MESH_H
#define MESH_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "glad/glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh
{
    // Mesh Data
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    
    Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
    {
        m_vertices = Vertices;
        m_indices = Indices;
        m_textures = Textures;
        
        setupMesh();
    }
    
    void Draw(Shader &shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        
        // Binding Textures
        for(unsigned int i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number{};
            std::string name = m_textures[i].type;
            if(name == "texture_diffuse")
                name = std::to_string(diffuseNr++);
            if(name == "texture_specular")
                name = std::to_string(specularNr++);
            
            shader.setInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }
        
        // Draw the Object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        
        glActiveTexture(GL_TEXTURE0);
    }
    
    private:
    unsigned int VAO, VBO, EBO;
    
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
        
        //Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        //Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);
        
        //Vertex TexCoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
};

#endif //MESH_H
