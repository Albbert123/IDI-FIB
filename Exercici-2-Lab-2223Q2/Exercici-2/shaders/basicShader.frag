#version 330 core

in vec3 fcolor;

out vec4 FragColor;

uniform vec3 colorcam2;
uniform float pinta;

void main() {

	if(pinta == 1) FragColor = vec4(0,0.7,0,1); //verd del terra (c*(0,1,0))
	else FragColor = vec4(fcolor*colorcam2, 1);
}
