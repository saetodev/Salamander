#include "Renderer.h"

#include <glad/glad.h>

#define MAX_QUAD_COUNT 256
#define MAX_TEXTURE_COUNT 8

#define VERTICES_PER_QUAD 6

struct Vertex {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    int texID;
};

static u32 m_vao    = 0;
static u32 m_vbo    = 0;
static u32 m_shader = 0;

static usize m_quadCount      = 0;
static Vertex* m_vertexBuffer = NULL;

//NOTE: this is the "unique" texture count (not the number of textured quads being drawn)
static usize m_textureCount = 0;
static Texture2D m_textureSlots[MAX_TEXTURE_COUNT];

static Texture2D m_whiteTexture;

static char* ReadEntireFile(const char* filename) {
    FILE* file = fopen(filename, "rb");

    if (!file) {
        //TODO: log this
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    usize size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = new char[size + 1];
    memset(buffer, 0, size);

    fread(buffer, size, 1, file);
    fclose(file);

    return buffer;
}

static u32 CreateShader(const char* vertexSource, const char* fragmentSource) {
    int result        = 0;
	char infoLog[512] = {};

	u32 program = glCreateProgram();

    // create vertex shader
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        printf("ERROR: Failed to compile vertex shader\n%s\n", infoLog);
	}

    // create fragment shader
	uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        printf("ERROR: Failed to compile fragment shader\n%s\n", infoLog);
	}

    // link program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
        printf("ERROR: Failed to link shader program\n%s\n", infoLog);
	}

	glValidateProgram(program);

    // validation
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
        printf("ERROR: Failed to validate shader program\n%s\n", infoLog);
	}

	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);

    return program;
}

static void Flush() {
    usize vertexBufferSize = sizeof(Vertex) * VERTICES_PER_QUAD * m_quadCount;
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, m_vertexBuffer);
    
    for (int i = 0; i < m_textureCount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureSlots[i].id);
        
        char buff[128] = {};
        sprintf(buff, "u_textureSlots[%d]", i);
        int loc = glGetUniformLocation(m_shader, buff);
        glUniform1i(loc, i);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, m_quadCount * VERTICES_PER_QUAD);

    m_quadCount    = 0;
    m_textureCount = 0;
}

void Renderer2D::Init() {
    // init vertex array
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // init vertex buffer
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID));
    
    m_vertexBuffer = new Vertex[MAX_QUAD_COUNT * VERTICES_PER_QUAD];

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_QUAD_COUNT * VERTICES_PER_QUAD, nullptr, GL_DYNAMIC_DRAW);

    // init shader
    char* vertexShaderSource   = ReadEntireFile("assets/quad_vertex.glsl");
    char* fragmentShaderSource = ReadEntireFile("assets/quad_fragment.glsl");

    m_shader = CreateShader(vertexShaderSource, fragmentShaderSource); 
    glUseProgram(m_shader);

    delete[] vertexShaderSource;
    delete[] fragmentShaderSource;

    // init textures
    u32 pixels[] = { 0xFFFFFFFF };
    m_whiteTexture = CreateTexture(1, 1, &pixels);
}

void Renderer2D::Shutdown() {
    DestroyTexture(m_whiteTexture);

    delete[] m_vertexBuffer;

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Renderer2D::Start(const glm::mat4& projection) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glUseProgram(m_shader);

    int loc = glGetUniformLocation(m_shader, "u_projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);
}

void Renderer2D::End() {
    Flush();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Renderer2D::DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color) {
#if 0
    if (m_quadCount == MAX_QUAD_COUNT) {
        Flush();   
    }

    int offset = m_quadCount * VERTICES_PER_QUAD;

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    glm::mat4 transform = translate * scale;

    static glm::vec4 quadVertices[] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },

        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
    };

    for (int i = 0; i < VERTICES_PER_QUAD; i++) {
        m_vertexBuffer[offset + i].position = transform * quadVertices[i];
        m_vertexBuffer[offset + i].color    = color; 
    }

    m_quadCount += 1;
#endif

    DrawTexture(m_whiteTexture, position, size, color);
}

void Renderer2D::DrawTexture(const Texture2D& tex, glm::vec2 position, glm::vec2 size, glm::vec4 color) {
    if (m_quadCount == MAX_QUAD_COUNT || m_textureCount == MAX_TEXTURE_COUNT) {
        Flush();
    }
    
    int offset = m_quadCount * VERTICES_PER_QUAD;

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    glm::mat4 transform = translate * scale;

    int texID = -1;

    for (int i = 0; i < m_textureCount; i++) {
        if (m_textureSlots[i].id == tex.id) {
            texID = i;
            break;
        }
    }

    if (texID == -1) {
        texID = m_textureCount;
        m_textureSlots[m_textureCount] = tex;
        m_textureCount += 1;
    }

    static glm::vec4 quadVertices[] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },

        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
    };

    static glm::vec2 quadTexCoords[] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },

        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
    };

    for (int i = 0; i < VERTICES_PER_QUAD; i++) {
        m_vertexBuffer[offset + i].position  = transform * quadVertices[i];
        m_vertexBuffer[offset + i].color     = color;
        m_vertexBuffer[offset + i].texCoords = quadTexCoords[i];
        m_vertexBuffer[offset + i].texID     = texID; 
    }

    m_quadCount += 1;
}
