#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

//Definir une matrice MVP

//out vec4 vertexColor;

void main(){

	gl_Position = vec4(vertexPosition_modelspace, 1.0);

	//vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}

