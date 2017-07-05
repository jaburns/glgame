#include "skybox.hpp"

#include <glm/gtc/type_ptr.hpp>

static GLfloat skybox_vertices[] = {
    -1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
};

SkyboxRenderer::SkyboxRenderer()
{
    m_program = std::make_unique<const ShaderProgram>("res/shaders/skybox.vert", "res/shaders/skybox.frag");;
    m_cubemap = std::make_unique<const CubeMap>(
        "res/hw_crater/craterlake_rt.png",
        "res/hw_crater/craterlake_lf.png",
        "res/hw_crater/craterlake_up.png",
        "res/hw_crater/craterlake_dn.png",
        "res/hw_crater/craterlake_bk.png",
        "res/hw_crater/craterlake_ft.png"
    );

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(*m_program, "position"));
    glVertexAttribPointer(glGetAttribLocation(*m_program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
}

SkyboxRenderer::~SkyboxRenderer()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vertex_buffer);
}

void SkyboxRenderer::draw_once(const glm::mat4x4& view, const glm::mat4x4& projection)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *m_cubemap);

    glDepthFunc(GL_LEQUAL);

    glUseProgram(*m_program);
    auto trunc_view = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(*m_program, "view"), 1, GL_FALSE, glm::value_ptr(trunc_view));
    glUniformMatrix4fv(glGetUniformLocation(*m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(*m_program, "skybox"), 0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}