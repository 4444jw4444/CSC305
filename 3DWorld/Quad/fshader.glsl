#version 330 core
out vec3 color;
in vec3 fpoint;
in float vheight; ///< for debug coloring
in float scale; ///< to normalize color range
in vec2 uv;
in float scaleB;
uniform sampler2D tex;
uniform sampler2D snow;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D water;

//uniform sampler2D tex;

vec3 Red = vec3(1,0,0);
vec3 Green = vec3(0,1,0);
vec3 Blue = vec3(0,0,1);
vec3 White = vec3(1,1,1);
vec3 Black = vec3(0,0,0);
vec3 Brown = vec3(1,0.5,0.5);
vec3 normal;
float dot;
float vdot;
vec3 vertical = vec3(0,0,1);
vec3 ex;
vec3 ey;
vec3 light = vec3(0,-1,1);
float tex_at(vec2 uv){ return texture(tex,uv).b; }
float stepx;
float stepy;
void main() {
    vec2 size = textureSize(tex,0);
    stepx = 2/size[0];
    stepy = 2/size[1];
    ex = fpoint - vec3(fpoint.x + stepx, fpoint.y, scale * tex_at(scaleB*vec2(fpoint.x + stepx, fpoint.y)+scaleB));
    ey = fpoint - vec3(fpoint.x, fpoint.y + stepy, scale * tex_at(scaleB*vec2(fpoint.x, fpoint.y + stepy)+scaleB));
    normal = normalize(cross(ex,ey));
    dot = dot(normal,normalize(light));
    vdot = dot(normal,normalize(vertical));
    //color = vec3(0,0,0);
    vec4 texColor;
    int tiling = 25;
    if(fpoint.z < 0.005){//water
        texColor = texture(water,vec2(fpoint.x,fpoint.y)*tiling);
        //color = mix(0.8*vec3(texColor.r, texColor.g, texColor.b),vec3(texColor.r, texColor.g, texColor.b),0.01*vheight);
        color = mix(White,vec3(texColor.r, texColor.g, texColor.b),1);
       if(fpoint.z < 0.001){
           color = 0.85*vec3(texColor.r, texColor.g, texColor.b);
       }
    }else{//land
        if(vdot < 0.6){//rock
            texColor = texture(rock,vec2(fpoint.x,fpoint.y)*tiling);
        }else{
            if(fpoint.z > 0.005 && fpoint.z < 0.02){//sand
                texColor = texture(sand,vec2(fpoint.x,fpoint.y)*tiling);
            }
            else if(fpoint.z > 0.02 && fpoint.z < 0.09){//grass
                texColor = texture(grass,vec2(fpoint.x,fpoint.y)*tiling);
            }
            else if(fpoint.z > 0.08){//snow
                texColor = texture(snow,vec2(fpoint.x,fpoint.y)*tiling);
            }
        }
        color = dot*mix(White,vec3(texColor.r, texColor.g, texColor.b),1);
    }
}


