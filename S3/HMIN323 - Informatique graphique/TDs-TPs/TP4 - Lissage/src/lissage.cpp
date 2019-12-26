// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <float.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
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

#include <algorithm>

/******************************************************************************/
/***************           Fonctions à completer         **********************/
/******************************************************************************/



/******************************************************************************/

void compute_triangle_normals (const std::vector<glm::vec3> & vertices,
                               const std::vector<std::vector<unsigned short> > & triangles,
                               std::vector<glm::vec3> & triangle_normals){

    for (unsigned int i = 0 ; i < triangles.size() ; i ++) {
        const std::vector<unsigned short> & ti = triangles[i];
        glm::vec3 e01 = vertices[ti[1]] - vertices[ti[0]];
        glm::vec3 e02 = vertices[ti[2]] - vertices[ti[0]];
        glm::vec3 n = glm::cross(e01, e02);
        n = glm::normalize (n);
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP3 - Lissage", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glDisable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

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
    for( unsigned int i = 0 ; i < indexed_vertices.size() ; i++ ){
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
    for( unsigned int i = 0 ; i < valences.size() ; i++ ){
        max_valence = std::max(max_valence, valences[i]);
    }


    std::vector<float> valence_field(valences.size());
    for( unsigned int i = 0 ; i < valences.size() ; i++ ){
        valence_field[i] = valences[i]/float(max_valence);
    }

    //****************************************************************/

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    GLuint valencesbuffer;
    glGenBuffers(1, &valencesbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, valencesbuffer);
    glBufferData(GL_ARRAY_BUFFER, valence_field.size() * sizeof(float), &valence_field[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        // Camera matrix
        glm::mat4 ViewMatrix       = glm::lookAt(
                    glm::vec3(0,0,3), // Camera is at (4,3,3), in World Space
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

        glm::vec3 lightPos = glm::vec3(4,4,4);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                    1,                                // attribute
                    2,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void*)0                          // array buffer offset
                    );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
                    2,                                // attribute
                    3,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void*)0                          // array buffer offset
                    );

        // 4th attribute buffer : valences
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, valencesbuffer);
        glVertexAttribPointer(
                    3,                                // attribute
                    1,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void*)0                          // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &valencesbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

/*

glfw
antweak
glfw
assimp
bullet
 */