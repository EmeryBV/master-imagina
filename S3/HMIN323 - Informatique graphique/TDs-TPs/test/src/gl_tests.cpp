// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <float.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h> // Wrapper portable d'OpenGL
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/maillage.hpp>

#include <algorithm>
#include <optional>
#include <array>
#include <tuple>
#include <type_traits>

#include <boost/pfr.hpp>


/******************************************************************************/
/***************           Fonctions à completer         **********************/
/******************************************************************************/

// tests pour le devellopements de mes fonctions

template<class T>
GLenum to_GLenum();
template<> GLenum to_GLenum<char>()           { return GL_BYTE;           }
template<> GLenum to_GLenum<unsigned char>()  { return GL_UNSIGNED_BYTE;  }
template<> GLenum to_GLenum<short>()          { return GL_SHORT;          }
template<> GLenum to_GLenum<unsigned short>() { return GL_UNSIGNED_SHORT; }
template<> GLenum to_GLenum<int>()            { return GL_INT;            }
template<> GLenum to_GLenum<unsigned int>()   { return GL_UNSIGNED_INT;   }
template<> GLenum to_GLenum<float>()          { return GL_FLOAT;          }
template<> GLenum to_GLenum<double>()         { return GL_DOUBLE;         }

template<int N, typename... Ts> using nth_element = typename std::tuple_element_t< N, std::tuple<Ts...> >;

template<typename T,
         typename std::enable_if_t<std::is_arithmetic_v<T>, T>* = nullptr>
void assign_attribute_type(size_t attribute_index, bool normalized = false)
{
    std::cerr << "Assigning arithmetic attribute...\n";
    glVertexAttribPointer(
        attribute_index,
        1,
        to_GLenum< T >(),
        normalized,
        0,
        (const void*)0);
}

template<typename T,
         typename std::enable_if_t<std::is_aggregate_v<T>, T>* = nullptr>
void assign_attribute_type(size_t attribute_index, bool normalized = false)
{
    std::cerr << "Assigning aggregate attribute...\n";
    glVertexAttribPointer(
        attribute_index,
        boost::pfr::tuple_size_v<T>,
        to_GLenum< boost::pfr::tuple_element_t<0, T> >(),
        normalized,
        0,
        (const void*)0);
}

template<typename T,
         typename std::enable_if_t<std::is_class_v<T> && !std::is_aggregate_v<T>, T>* = nullptr> 
void assign_attribute_type(size_t attribute_index, bool normalized = false)
{
    std::cerr << "Assigning container attribute...\n";
    glVertexAttribPointer(
        attribute_index,
        sizeof(typename T::type) / sizeof(typename T::value_type),
        to_GLenum< typename T::value_type >(),
        normalized,
        0,
        (const void*)0);
}

template<GLenum BINDING_TARGET, typename T>
class GenericBuffer
{
protected:

    unsigned int m_id;

public:

    using value_type = T;
    // using binding_target = BINDING_TARGET;

    // Copy impossible
    GenericBuffer(const GenericBuffer&)  = delete;
    void operator=(const GenericBuffer&) = delete;

    GenericBuffer()
    {
        std::cerr << "GenericBuffer()\n";
        glGenBuffers(1, &m_id);
    }

    GenericBuffer(const std::vector<T>& data, GLenum BUFFER_USAGE = GL_STATIC_DRAW) : GenericBuffer()
    {
        this->bind();
        this->assign(data, BUFFER_USAGE);
    }

    ~GenericBuffer()
    {
        std::cerr << "~GenericBuffer()\n";
        glDeleteBuffers(1, &m_id);
    }

    unsigned int id() const
    {
        return m_id;
    }

    void bind() const
    {
        glBindBuffer(BINDING_TARGET, this->id());
    }

    void assign(const std::vector<T>& data, GLenum BUFFER_USAGE = GL_STATIC_DRAW) const
    {
        glBufferData(BINDING_TARGET, data.size() * sizeof(T), data.data(), BUFFER_USAGE);
    }

    void update(size_t offset, const std::vector<T>& data) const
    {
        glBufferSubData(BINDING_TARGET, offset, data.size() * sizeof(T), data.data());
    }
};

template<typename T>
using VBO = GenericBuffer<GL_ARRAY_BUFFER, T>;

template<typename... Types>
using VBOs = std::tuple<VBO<Types>...>;

template<typename T>
using EBO = GenericBuffer<GL_ELEMENT_ARRAY_BUFFER, T>;

template<typename... Types>
class GenericVertexArray
{
protected:

    unsigned int m_id;
    // std::optional<VBOs<Types...>> vbos;

public:

    // Copy impossible
    GenericVertexArray(const GenericVertexArray&) = delete;
    void operator=(const GenericVertexArray&) = delete;

    GenericVertexArray()
    {
        std::cerr << "GenericVertexArray()\n";
        glGenVertexArrays(1, &m_id);
    }
    /*
    template<typename Tuple, std::size_t... Is>
    void construct_impl(Tuple& t, std::index_sequence<Is...>)
    {
        (this->assign<Is>(std::get<Is>(t)), ...);
    }

    GenericVertexArray(VBO<Types>&... vbos)
    {
        this->bind();
        construct_impl(t, std::index_sequence_for<Types...>{});
    }
    */
    ~GenericVertexArray()
    {
        std::cerr << "~GenericVertexArray()\n";
        glGenVertexArrays(1, &m_id);
    }

    size_t size() const
    {
        return sizeof...(Types);
    }

    unsigned int id() const
    {
        return m_id;
    }

    void bind()
    {
        glBindVertexArray(this->id());
    }

    template<size_t INDEX>
    void assign(const VBO< nth_element<INDEX, Types...> >& vbo, bool normalized = true) const
    {
        std::cerr << typeid(nth_element<INDEX, Types...>).name() << '\n';

        vbo.bind();
        assign_attribute_type< nth_element<INDEX, Types...>  >(INDEX, normalized);
    }

    template<size_t INDEX>
    void enable() const
    {
        static_assert(INDEX < sizeof...(Types), "enable() index too big");
        glEnableVertexAttribArray(INDEX);
    }

    template<size_t INDEX>
    void disable() const
    {
        static_assert(INDEX < sizeof...(Types), "disable() index too big");
        glDisableVertexAttribArray(INDEX);
    }
};

template<typename... Types>
using VAO = GenericVertexArray<Types...>;

/******************************************************************************/

void compute_triangle_normals (const std::vector<glm::vec3> & vertices,
                               const std::vector<std::vector<unsigned short> > & triangles,
                               std::vector<glm::vec3> & triangle_normals){

    for (unsigned int i = 0 ; i < triangles.size() ; i ++) {
        const std::vector<unsigned short> & ti = triangles[i];
        glm::vec3 e01 = vertices[ti[1]] - vertices[ti[0]];
        glm::vec3 e02 = vertices[ti[2]] - vertices[ti[0]];
        glm::vec3 n = glm::cross(e01, e02);
        n = glm::normalize(n);
        triangle_normals.push_back (n);
    }

}

void compute_smooth_vertex_normals (const std::vector<glm::vec3> & vertices,
                                    const std::vector<std::vector<unsigned short> > & triangles,
                                    unsigned int weight_type,
                                    std::vector<glm::vec3> & vertex_normals){

    vertex_normals.clear();
    for ( unsigned int i=0; i < vertices.size(); i++){
        vertex_normals.push_back(glm::vec3(0., 0., 0.));
    }
    std::vector<glm::vec3> triangle_normals;
    compute_triangle_normals (vertices, triangles, triangle_normals);

    for ( unsigned int t_id = 0 ; t_id < triangles.size(); t_id++){
        const glm::vec3 &t_normal = triangle_normals[t_id];
        const std::vector<unsigned short> &t = triangles[t_id];
        for (unsigned int  j = 0; j < 3; j++) {
            const glm::vec3 &vj_pos = vertices[ t[j] ];
            unsigned short vj = t[j];
            float w = 1.0; // uniform weights
            glm::vec3 e0 = vertices[t[(j+1)%3]] - vj_pos;
            glm::vec3 e1 = vertices[t[(j+2)%3]] - vj_pos;
            if (weight_type == 1) { // area weight
                w = glm::length(glm::cross (e0, e1))/2.;
            } else if (weight_type == 2) { // angle weight
                e0 = glm::normalize(e0);
                e1 = glm::normalize(e1);
                w = (2.0 - (glm::dot (e0, e1) + 1.0)) / 2.0;
            }
            if (w <= 0.0)
                continue;
            vertex_normals[vj] =vertex_normals[vj] + t_normal * w;
        }


    }

    for ( unsigned int i=0; i < vertex_normals.size(); i++){
        vertex_normals[i] = glm::normalize(vertex_normals[i]);
    }
}

void collect_one_ring (const std::vector<glm::vec3> & vertices,
                       const std::vector<std::vector<unsigned short> > & triangles,
                       std::vector<std::vector<unsigned short> > & one_ring) {

    one_ring.resize (vertices.size ());
    for (unsigned int i = 0; i < triangles.size (); i++) {
        const std::vector<unsigned short> & ti = triangles[i];
        for (unsigned int j = 0; j < 3; j++) {
            unsigned short vj = ti[j];
            for (unsigned int k = 1; k < 3; k++) {
                unsigned int vk = ti[(j+k)%3];
                if (std::find (one_ring[vj].begin (), one_ring[vj].end (), vk) == one_ring[vj].end ())
                    one_ring[vj].push_back (vk);
            }
        }
    }
}


void compute_vertex_valences (const std::vector<glm::vec3> & vertices,
                              const std::vector<std::vector<unsigned short> > & triangles,
                              std::vector<unsigned int> & valences ) {

    std::vector<std::vector<unsigned short> > one_ring;

    collect_one_ring( vertices, triangles, one_ring );

    valences.clear();
    valences.resize(vertices.size());

    for( unsigned int i = 0 ; i < vertices.size() ; i++ )
        valences[i] = one_ring[i].size();

}

// void calc_uniform_mean_curvature()

/*******************************************************************************/

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP3 - Lissage", NULL, NULL);

    if( window == NULL )
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glDisable(GL_CULL_FACE);



    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Load the texture
    GLuint Texture = loadDDS("uvmap.DDS");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;

    //Chargement du fichier de maillage
    std::string filename("suzanne.off");
    loadOFF(filename, indexed_vertices, indices, triangles );
    indexed_uvs.resize(indexed_vertices.size(), glm::vec2(1.)); //List vide de UV

    glm::vec3 bb_min( FLT_MAX, FLT_MAX, FLT_MAX );
    glm::vec3 bb_max( FLT_MIN, FLT_MIN, FLT_MIN );

    //Calcul de la boite englobante du modèle
    for( unsigned int i = 0 ; i < indexed_vertices.size() ; i++ )
    {
        bb_min = glm::min(bb_min, indexed_vertices[i]);
        bb_max = glm::max(bb_max, indexed_vertices[i]);
    }

    glm::vec3 size = bb_max - bb_min;
    glm::vec3 center = glm::vec3(bb_min.x + size.x/2, bb_min.y + size.y/2 , bb_min.z + size.z/2 );
    float model_scale = 2.0/std::max( std::max(size.x, size.y), size.z );

    //****************************************************************/
    //Calculer les normales par sommet
    // indexed_normals.resize(indexed_vertices.size(), glm::vec3(1.));
    compute_smooth_vertex_normals(indexed_vertices, triangles, 2, indexed_normals);

    std::vector<unsigned int> valences;
    compute_vertex_valences( indexed_vertices, triangles, valences );

    unsigned int max_valence = 0;
    for( unsigned int i = 0 ; i < valences.size() ; i++ )
    {
        max_valence = std::max(max_valence, valences[i]);
    }

    std::vector<float> valence_field(valences.size());
    for( unsigned int i = 0 ; i < valences.size() ; i++ )
    {
        valence_field[i] = valences[i]/float(max_valence);
    }

    // VAO
    
    VBO<glm::vec2> uvs_buffer(indexed_uvs);
    VBO<glm::vec3> vertices_buffer(indexed_vertices);
    VBO<glm::vec3> normals_buffer(indexed_normals);
    VBO<float>     valences_buffer(valence_field);

    /*
    EBO<unsigned short> indices_buffer(indices);

    VAO<glm::vec3, glm::vec2, glm::vec3> vertex_array(vertices_buffer, uvs_buffer, normals_buffer);
    */

    VAO<glm::vec3, glm::vec2, glm::vec3> vertex_array;
    vertex_array.bind();

    vertex_array.assign<0>(vertices_buffer);
    vertex_array.assign<1>(uvs_buffer);
    vertex_array.assign<2>(normals_buffer);

    EBO<unsigned short> indices_buffer(indices);
    // indices_buffer.bind();

    vertex_array.enable<0>();
    vertex_array.enable<1>();
    vertex_array.enable<2>();
    // vertex_array.enable<3>();

    // vertex_array.assign_and_enable<decltype(valences_buffer)::value_type>(3);
    
    
    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

// maverick.inria.fr/stages

    do
    {
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Simplification du maillage
        if (glfwGetKey(window, GLFW_KEY_S))
        {

        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Use our shader
        glUseProgram(programID);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 ViewMatrix       = glm::lookAt(
                    glm::vec3(3 * sin(currentTime),0,3 * cos(currentTime)), // Camera is at (4,3,3), in World Space
                    glm::vec3(0,0,0), // and looks at the origin
                    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                    );
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 ModelMatrix      = glm::scale(glm::mat4(1.0f), glm::vec3(model_scale))*glm::translate(glm::mat4(1.0f), glm::vec3(-center.x, -center.y, -center.z));
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glm::vec3 lightPos = glm::vec3(4 * sin(currentTime),4,4 * cos(currentTime));
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        vertex_array.bind();

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,       // mode
                    indices.size(),     // count
                    GL_UNSIGNED_SHORT,  // type
                    (void*)0            // element array buffer offset
                    );

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
