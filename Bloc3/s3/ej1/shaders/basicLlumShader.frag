#version 330 core

in vec3 matamb2;
in vec3 matdiff2;
in vec3 matspec2;
in float matshin2;

in vec4 fvertexSCO;
in vec3 fnormalSCO;

uniform vec3 colorFocus;
uniform vec3 colorFocus2;
uniform vec3 llumAmbient;
uniform vec3 posFocus;  // en SCA
uniform vec3 posFocusO;  // en SCO

uniform int focusEsc; //bool

uniform mat4 view;

out vec4 FragColor;


vec3 Ambient() {
    return llumAmbient * matamb2;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiff2 * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshin2 == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin2);
    return (matspec2 * colFocus * shine); 
}


void main()
{	
	//volver a normalizar fnormalSCO 
	vec3 fnormalSCO2 = normalize(fnormalSCO);

  vec4 posFocus1;
  vec4 posFocus2;
	posFocus1 = view * vec4(posFocus,1.0); //focus Escena
  posFocus2 = vec4(posFocusO,1.0); //focus camara

	vec3 L1 = normalize((posFocus1 - fvertexSCO).xyz);
  vec3 L2 = normalize((posFocus2 - fvertexSCO).xyz);

	vec3 difus1 = Difus(fnormalSCO2,L1,colorFocus);
  vec3 difus2 = Difus(fnormalSCO2,L2,colorFocus2);

  vec3 ambient = Ambient();
  vec3 Lambert = ambient + difus1 + difus2;

	vec3 Phong1 = Especular(fnormalSCO2,L1,fvertexSCO,colorFocus);
  vec3 Phong2 = Especular(fnormalSCO2,L2,fvertexSCO,colorFocus2);

	FragColor = vec4(Lambert + Phong1 + Phong2,1);
}




/*
Foco fijo--> SCA --> xView
Foco pegado a modelo --> SCM --> View x TG




*/