mat4 u_mvpMatrix = mat4( 2.0/320.0, 0.0, 0.0, -1.0,
                             0.0, -2.0/480.0, 0.0, 1.0,
                             0.0, 0.0, -1.0, 0.0,
                             0.0, 0.0, 0.0, 1.0);   

attribute vec4 g_position;
attribute vec4 g_texCoords;
attribute vec4 g_color;

varying vec2 textureCoordinate;

void main()
{
	vec4 pos = vec4(g_position.xyz, 1);
   gl_Position = u_mvpMatrix * pos;
   textureCoordinate = g_texCoords.xy;
}