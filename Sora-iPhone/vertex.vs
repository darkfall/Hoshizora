attribute vec4 position;
attribute vec4 color;
attribute vec2 uv;

varying vec4 colorVarying;
varying vec2 texcoord;

uniform mat4 mvpmatrix;

mat4 projection() {
    return mat4(vec4(2.0/320.0, 0, 0, -1.0), 
                vec4(0, 2.0/-480.0, 0.0, 1.0), 
                vec4(0.0, 0.0, -2, -1.0), 
                vec4(0.0, 0.0, 0.0, 1.0)
                        );
}

mat3 view() {
    return mat3(
                vec3(1.0/160.0, 0.0, -1.0),
                vec3(0.0, -1.0/240.0, 1.0),
                vec3(0.0, 0.0, 1.0)
                );
}

mat4 scale(float x, float y, float z)
{
    return mat4(
                vec4(x,   0.0, 0.0, 0.0),
                vec4(0.0, y,   0.0, 0.0),
                vec4(0.0, 0.0, z,   0.0),
                vec4(0.0, 0.0, 0.0, 1.0)
                );
}

mat4 translate(float x, float y, float z)
{
    return mat4(
                vec4(1.0, 0.0, 0.0, 0.0),
                vec4(0.0, 1.0, 0.0, 0.0),
                vec4(0.0, 0.0, 1.0, 0.0),
                vec4(x,   y,   z,   1.0)
                );
}

mat4 rotate_x(float theta)
{
    return mat4(
                vec4(1.0,         0.0,         0.0, 0.0),
                vec4(0.0,  cos(theta),  sin(theta), 0.0),
                vec4(0.0, -sin(theta),  cos(theta), 0.0),
                vec4(0.0,         0.0,         0.0, 1.0)
                );
}

void main()
{
    texcoord = uv;

    gl_Position = projection() * position ;
    gl_Position.w = 1.0;
    
    colorVarying = color;
}
