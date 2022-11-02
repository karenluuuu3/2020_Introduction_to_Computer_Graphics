#version 430 

layout (triangles) in;
layout (triangle_strip, max_vertices = 200) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

uniform vec3 time;
float t = time[0]*1000;
out vec2 TexCoords;


vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 10.0f;
    vec3 direction = normal * ((sin(t) + 1.0f) / 2.0f) * magnitude;
    return position + vec4(direction, 0.0f);
}

vec3 GetNormal(int k)
{
   vec3 a = vec3(gl_in[k].gl_Position) - vec3(gl_in[k+1].gl_Position);
   vec3 b = vec3(gl_in[k+2].gl_Position) - vec3(gl_in[k+1].gl_Position);
   return normalize(cross(a, b));
}

void main() 
{
    for( int i=0; i<gl_in.length(); i++ ){
        vec3 normal = GetNormal(i);
        gl_Position = explode(gl_in[i].gl_Position, normal);
        TexCoords = gs_in[i].texCoords;
        EmitVertex();
    }
    EndPrimitive();
}