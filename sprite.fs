#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D imageY;
uniform sampler2D imageU;
uniform sampler2D imageV;

void main() {
	float y = texture2D(imageY, TexCoords).r;
	float u = texture2D(imageU, TexCoords).r - 0.5;
	float v = texture2D(imageV, TexCoords).r - 0.5;
	
	float r = y + 1.402 * v;  
    float g = y - 0.344 * u - 0.714 * v;  
    float b = y + 1.772 * u;  

    color = vec4(r, g, b, 1.0);
}
