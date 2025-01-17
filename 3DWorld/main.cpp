#include "icg_common.h"
#include "Quad/Quad.h"
#include "Perlin/Perlin.h"

Quad quad;
Perlin perlin;
float theta = 30;
float camX = 0;
float camY = 0;
float camZ = 0;
float rateX = 0;
float rateY = 0;
float rateZ = 0;
float lookX = 0;
float lookY = 0;
float lookZ = 0;
int windowDimension = 880;
///Initializes the image by performing all perlin, mesh, heightmap, and other calls that generate the terrain.
void init(){
    glfwEnable(GLFW_KEY_REPEAT);
    ///--- Initializes the Perlin noise texture
    ///--- Initializes the fBm texture
    ///--- Sets background color
    glClearColor(/*gray*/ .8,.8,.8, /*solid*/1.0 );
    glEnable(GL_DEPTH_TEST);
    ///--- Initlializes the terrain quad mesh (?takes the fBm as an input for displacing the height of vertices?)
    quad.init(perlin.init());
}

///Displays
void display(){
    ///--- Clears the color buffer so that we don't display garbage
    glViewport(0,0,windowDimension,windowDimension);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///--- Upload viewing matrices externally
        GLuint pid = quad.getProgramID();
        glUseProgram(pid);
            mat4 MODEL = mat4::Identity();
            glUniformMatrix4fv(glGetUniformLocation(pid, "MODEL"), 1, GL_FALSE, MODEL.data());

            float theta_rad = M_PI/180.0*theta;
            //vec3 camera_pos( camX, camY, 2*sin(theta_rad));
            camX = camX + rateX;
            camY = camY + rateY;
            camZ = camZ + rateZ;
            vec3 camera_pos(camX,camY,camZ);
            mat4 VIEW = Eigen::lookAt( camera_pos, vec3(lookX,lookY,lookZ), vec3(0,0,1) ); //< "z" up on screen
            glUniformMatrix4fv(glGetUniformLocation(pid, "VIEW"), 1, GL_FALSE, VIEW.data());

            mat4 PROJ = Eigen::perspective(75.0f, windowDimension/(float)windowDimension, 0.1f, 10.0f);
            glUniformMatrix4fv(glGetUniformLocation(pid, "PROJ"), 1, GL_FALSE, PROJ.data());
        glUseProgram(pid);
    ///--- calls the quad.draw() method which displays the terrain quad (?takes in the ModelViewProjection matrices?)
    quad.draw();
}

/// NOTE: see glfwEnable(GLFW_KEY_REPEAT)
void keyboard(int key, int action){
    if (action==GLFW_PRESS && key==GLFW_KEY_SPACE)
            quad.wireframe = !quad.wireframe;
    //camera position
    if (action==GLFW_PRESS && key==GLFW_KEY_UP){
        rateY += 0.001;
    }
        //theta = std::min( theta+1, +89.f );
    if (action==GLFW_PRESS && key==GLFW_KEY_DOWN){
        rateY -= 0.001;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==GLFW_KEY_LEFT){
        rateX += 0.001;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==GLFW_KEY_RIGHT){
        rateX -= 0.001;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==49){
        rateZ += 0.001;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==50){
        rateZ -= 0.001;
    }

    //lookat position
    if (action==GLFW_PRESS && key==51){
        lookX += 0.05;
    }
        //theta = std::min( theta+1, +89.f );
    if (action==GLFW_PRESS && key==52){
        lookX -= 0.05;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==53){
        lookY += 0.05;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==54){
        lookY -= 0.05;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==55){
        lookZ += 0.05;
    }
        //theta = std::max( theta-1, -89.f );
    if (action==GLFW_PRESS && key==56){
        lookZ -= 0.05;
    }
//    GLFW_MOUSE_BUTTON_LEFT
//    if ()

}

int main(int, char**){
    ///--- Set window size
    glfwInitWindowSize(windowDimension, windowDimension);
    ///--- Create the window
    glfwCreateWindow("3DWorld");
    ///--- Read in the keyboard input and modify the camera's position
    glfwSetKeyCallback(keyboard);
    ///--- Specify and call the display function
    glfwDisplayFunc(display);
    ///--- Initialize the window
    init();
    ///--- Set the main loop
    glfwMainLoop();
    ///--- Cleanup after our project exits the glfwMainLoop
    quad.cleanup();
    ///--- Exit the main loop
    return EXIT_SUCCESS;
}
