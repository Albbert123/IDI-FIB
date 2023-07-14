#version 330 core

in vec4 vertexSCO;
in vec3 normalSCO;
in vec3 fmatamb;
in vec3 fmatdiff;
in vec3 fmatspec;
in float fmatshin;

out vec4 FragColor;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 colFocus = vec3(1, 1, 1);

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 posFocus;

uniform float canvifoco;

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * fmatamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * fmatdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (fmatshin == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + fmatspec * colFocus * shine); 
}

void main()
{	
    vec3 normalSCO2 = normalize(normalSCO);

    vec4 posFocus1;
    if(canvifoco == 0) posFocus1 = vec4(posFocus,1); //foco camera
    else posFocus1 = view * vec4(posFocus,1); //foco escena

    vec3 L = normalize((posFocus1 - vertexSCO).xyz);

    vec3 lambert = Lambert(normalSCO2,L);

    vec3 phong = Phong(normalSCO2, L, vertexSCO);

    FragColor = vec4(phong,1);
}
