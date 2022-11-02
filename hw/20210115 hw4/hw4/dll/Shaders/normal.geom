#version 430 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 P;
uniform vec3 time, ff;

float t = time[0];
out vec4 color;

void GenerateLine(int index)
{
    if(index==0) color=vec4(abs(sin(t)), abs(cos(t)), abs(tan(t)), t);
    else if(index==1) color=vec4(abs(tan(t)), abs(sin(t)), abs(cos(t)), t);
    else color=vec4(abs(cos(t)), abs(tan(t)), abs(sin(t)), t);

    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * 0.3;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    for(int i=0;i<gl_in.length();i++){
        GenerateLine(i);
    }
}
