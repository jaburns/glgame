#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../glfw.hpp"
#include "../resources.hpp"
#include "../world.hpp"

class TeapotRenderer
{
    GLuint m_vao;
    GLuint m_vertex_buffer;
    GLuint m_normal_buffer;
    GLuint m_index_buffer;

    std::unique_ptr<const ShaderProgram> m_program;
    std::unique_ptr<const Texture> m_texture;
    std::shared_ptr<const CubeMap> m_skybox_cubemap;

    TeapotRenderer(const TeapotRenderer&) =delete;
    TeapotRenderer& operator=(const TeapotRenderer&) =delete;

public:
    TeapotRenderer(std::shared_ptr<const CubeMap> skybox_cubemap);
    ~TeapotRenderer();

    void use(const glm::vec3& camera_pos, const glm::mat4x4& view, const glm::mat4x4& projection);
    void draw(const Transform& transform);
};