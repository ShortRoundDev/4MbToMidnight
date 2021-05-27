#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 dist;

uniform sampler2D tex;

void main(){
    float z = min(0.7,
        max(
            0,
            round(10.0/(length(dist.xz)) - 1)/10.0
        )
    );
    FragColor = texture(tex, TexCoord) * vec4(z, z, min(1.0, z * 1.2), 1.0);
}