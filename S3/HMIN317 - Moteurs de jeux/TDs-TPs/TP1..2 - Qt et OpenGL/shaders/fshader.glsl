//#version 330

//in vec4 v_color;

//out vec4 f_color;
varying vec2 v_tex_coord;

uniform sampler2D u_heightmap;
uniform sampler2D u_grass;
uniform sampler2D u_rock;
uniform sampler2D u_snowrocks;

void main()
{
    float height = texture2D(u_heightmap, v_tex_coord).p;

    if (height < 0.1)
        gl_FragColor = texture2D(u_grass, v_tex_coord);
    else if (height < 0.4)
        gl_FragColor = texture2D(u_rock, v_tex_coord);
    else
        gl_FragColor = texture2D(u_snowrocks, v_tex_coord);

//    gl_FragColor = texture2D(u_heightmap, v_tex_coord);
}
