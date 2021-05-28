#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;
uniform float alpha;

void main(){
    
    vec4 t = texture(tex, TexCoord);
    if(t.a == 0.0)
        discard;
    FragColor = t;
}