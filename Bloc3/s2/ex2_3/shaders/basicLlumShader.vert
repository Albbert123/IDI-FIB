#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem del focus de llum
//vec3 colorFocus = vec3(0.8, 0.8, 0.8);
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
//vec3 posFocus = vec3(1, 1, 1);  // en SCA

out vec3 matamb2;
out vec3 matdiff2;
out vec3 matspec2;
out float matshin2;

out vec4 fvertexSCO;
out vec3 fnormalSCO;

void main()
{	

    matamb2 = matamb;
    matdiff2 = matdiff;
    matspec2 = matspec;
    matshin2 = matshin;

    //calcular el vertex SCO como vec4 
    fvertexSCO = view * TG * vec4 (vertex, 1.0);

    //calcular la normal SCO 
    mat3 NormalMatrix = inverse (transpose (mat3 (view * TG)));
    fnormalSCO = normalize(NormalMatrix*normal);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
