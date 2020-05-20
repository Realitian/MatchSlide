#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main() {
	vec4 yuv = texture(image, TexCoords);
    color = vec4(yuv.r, 0, 0, 1.0);
}
