#version 330 core

// Output data
out vec4 FragColor;

uniform vec4 shaderColor;

void main()
{

	// Output color = red 
	FragColor = shaderColor;
}
