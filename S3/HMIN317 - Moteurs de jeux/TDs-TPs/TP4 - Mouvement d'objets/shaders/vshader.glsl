//#version 330

attribute vec3 a_position;
attribute vec2 a_tex_coord;

//out vec4 v_color;

varying vec2 v_tex_coord;

uniform mat4 u_mvp_matrix;
uniform float u_height_factor;
uniform sampler2D u_heightmap;

//attribute vec4 a_position;
//attribute vec2 a_texcoord;

void main()
{
//    gl_Position = vec4(a_position, 1.0);
    float height = texture2D(u_heightmap, a_tex_coord).p * u_height_factor;

    gl_Position = u_mvp_matrix * vec4(a_position.x, a_position.y, a_position.z - height, 1.0);
//    v_color = vec4(a_color, 1.0);
    // Calculate vertex position in screen space

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_tex_coord = a_tex_coord;
}
 
