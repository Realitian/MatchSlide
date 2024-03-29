#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main() {
	vec4 tex = texture2D(image, TexCoords);
    color = vec4(tex.b, tex.g, tex.r, tex.a);
}
