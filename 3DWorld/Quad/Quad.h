#pragma once
#include "icg_common.h"

class Quad{
private:
    GLuint _vao; /// vertex array object
    GLuint _vbo; /// vertex buffer object
    GLuint _ibo; /// index buffer object
    GLuint _pid; /// GLSL shader program ID
    bool wireframe = true; /// bolean used to determine wireframe or filled rendering
    const int _numOfTris = 10; /// the number of triangles to render on a side (square terrain mesh)
public:
    ///--- Initialization of Quad
    void init(){

        ///--- Vertex Array Object creation
        /// We first generate a vertex array in the GPU
        /// Then bind that buffer to _vao
        /// Then check errors
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        check_error_gl();

        ///--- Vertex coordinates specification/generation
        /// We start with the simples strip of triangles, a four point quad
        /// We then produce the rest of the points based on the _numOfTris value through a for loop.
        std::vector<vec3> vpoint;
//        vpoint.push_back(vec3(-0.7f, -0.5f, 0.0f));
//        vpoint.push_back(vec3(+0.7f, -0.5f, 0.0f));
//        vpoint.push_back(vec3(-0.7f, +0.5f, 0.0f));
//        vpoint.push_back(vec3(+0.7f, +0.5f, 0.0f));
        /// The max positions of the top, bottom, left and right sides
        float maxXY = 0.5;
        /// The actual distance to be split up into segments
        float distanceXY = maxXY * 2;
        /// The size of the steps that we take between points
        float offsetXY = distanceXY/_numOfTris;
        /// Go through all _numOfTris x _numOfTris vertexs and push_back them into the vpoints vector
        /// Based on coords determined using the offsetXY and maxXY
        for(int i = 0; i < _numOfTris; i++){
            for(int j = 0; j < _numOfTris; j++){
                /// The x,y,z values of the vertex
                float x = -maxXY + offsetXY * i;
                float y = maxXY - offsetXY *j;
                float z = 0.0;
                vpoint.push_back(vec3(x,y,z));
                printf("vpoint x: %f, y: %f, z: 0\n", x, y);
            }
        }

        ///--- Vertex Buffer Object creation
        /// We first generate the vertex buffer
        /// Then bind that buffer using _vbo
        /// Then inject the data in a given format in glBufferData from the vpoint array
        /// Then check errors
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _numOfTris * _numOfTris * sizeof(vec3), &vpoint[0], GL_STATIC_DRAW);
        check_error_gl();

        ///--- Index Buffer Object population
        std::vector<unsigned int> indices;
        /// iterate through the indice locations
//        indices.push_back(0);
//        indices.push_back(1);
//        indices.push_back(2);
//        indices.push_back(3);
        /// Restart index for the triangle strip rendering
        int restartIndex = _numOfTris*_numOfTris;
        /// Run through all indices that we are going to use in the correct rendering order
        for(int i = 0; i < restartIndex; i ++){
            indices.push_back(i);
//            indices.push_back(i + _numOfTris + 1);
//            if((_numOfTris%i) == 0){
//                indices.push_back(_numOfTris*_numOfTris);
//            }
        }

        ///--- Index Buffer Object creation
        /// We first create a variable to use to connect the buffer generation with the buffer binding
        /// Then we generate the buffer
        /// Then we bind the buffer
        /// Then send the buffer the data inside the indices array with the settings that we want
        /// Then we check the errors
        GLuint _vbo_indices;
        glGenBuffers(1, &_vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        check_error_gl();

        ///--- Compile the shaders
        _pid = opengp::load_shaders("Quad/vshader.glsl", "Quad/fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);
        glUseProgram(_pid);
    }

    ///--- Cleanup the quad
    void cleanup(){
        /// Setting a binding to 0 frees it
        glBindVertexArray(0);
        /// Setting program binding to 0 frees it
        glUseProgram(0);
        /// Delete the vertex buffer
        glDeleteBuffers(1, &_vbo);
        /// Delete the vertex array
        glDeleteVertexArrays(1, &_vao);
    }
    
    ///--- Draw the quad
    void draw(){
        /// Binds the data that we setup in the init() to the shaders
        bindShader();
        /// Enable triangle strip looping using an index outside of the range of the indices
        glEnable(GL_PRIMITIVE_RESTART);
        /// Use _numOfTris squared to give us the restart index.
        glPrimitiveRestartIndex(_numOfTris*_numOfTris);
        /// being polygon mode which enables the wireframe/filled mode switching
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        /// draw the elements onto the image
        glDrawElements(GL_TRIANGLE_STRIP, _numOfTris*_numOfTris*2, GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
        /// unbind the shader
        unbindShader();
    }
private:
    ///--- Bind shader(s)
    void bindShader() {
        /// enable the opengl shaders
        glUseProgram(_pid);
        /// bind the vertex array object
        glBindVertexArray(_vao);
        /// check for error
        check_error_gl();

        ///--- Vertex Attribute ID for Positions
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        /// check that vpoint_id is greater or equal to 0
        if (vpoint_id >= 0) {
            glEnableVertexAttribArray(vpoint_id);
            check_error_gl();
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glVertexAttribPointer(vpoint_id, 3 /*vec3*/, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            check_error_gl();
        }
    }
    ///--- Unbind shader(s)
    void unbindShader() {
        GLint vpoint_id = glGetAttribLocation(_pid, "vpoint");
        if (vpoint_id >= 0)
            glDisableVertexAttribArray(vpoint_id);
        glUseProgram(0);
        glBindVertexArray(0);
    }
};