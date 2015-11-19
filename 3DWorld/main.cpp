#include "icg_common.h"
#include "Quad/Quad.h"
#include "Perlin/Perlin.h"

Quad quad;
Perlin perlin;

///Initializes the image by performing all perlin, mesh, heightmap, and other calls that generate the terrain.
void init(){
    ///--- Initializes the Perlin noise texture
    perlin.init();
    ///--- Initializes the fBm texture
    ///--- Sets background color
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    ///--- Initlializes the terrain quad mesh (?takes the fBm as an input for displacing the height of vertices?)
    //quad.init();
}

///Displays
void display(){
    ///--- Clears the color buffer so that we don't display garbage
    glClear(GL_COLOR_BUFFER_BIT);
    ///--- calls the quad.draw() method which displays the terrain quad (?takes in the ModelViewProjection matrices?)
    //quad.draw();
}

int main(int, char**){
    ///--- Set window size
    glfwInitWindowSize(880, 880);
    ///--- Create the window
    glfwCreateWindow("3DWorld");
    ///--- Specify and call the display function
    glfwDisplayFunc(display);
    ///--- Initialize the window
    init();
    ///--- Set the main loop
    glfwMainLoop();
    ///--- Cleanup after our project exits the glfwMainLoop
    //quad.cleanup();
    ///--- Exit the main loop
    return EXIT_SUCCESS;
}
