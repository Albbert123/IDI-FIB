#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;

in vec3  fvertex;
in vec3  fnormal;

uniform vec3 colorFocus;
uniform vec3 colorFocus1;
uniform vec3 colorFocus2;
uniform vec3 colorFocus3;
uniform vec3 colorFocus4;

uniform vec3 posFocus;  
uniform vec3 posFocus1; 
uniform vec3 posFocus2;
uniform vec3 posFocus3;
uniform vec3 posFocus4;

//uniform vec3 llumAmbient;
const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 TGbola;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  //volver a normalizar fnormalSCO 
	vec3 fnormal2 = normalize(fnormal);

  vec4 posFocusEscena;
  vec4 posFocusM1;
  vec4 posFocusM2;
  vec4 posFocusM3;
  vec4 posFocusM4;

	posFocusEscena = View * vec4(posFocus,1.0); 
  posFocusM1 = View * TGbola * vec4(posFocus1,1.0); 
  posFocusM2 = View * TGbola * vec4(posFocus2,1.0); 
  posFocusM3 = View * TGbola * vec4(posFocus3,1.0); 
  posFocusM4 = View * TGbola * vec4(posFocus4,1.0); 

  vec3 L = normalize((posFocusEscena).xyz - fvertex);
  vec3 L1 = normalize((posFocusM1).xyz - fvertex);
  vec3 L2 = normalize((posFocusM2).xyz - fvertex);
  vec3 L3 = normalize((posFocusM3).xyz - fvertex);
  vec3 L4 = normalize((posFocusM4).xyz - fvertex);

  vec3 difus = Difus(fnormal2,L,colorFocus);
  vec3 difus1 = Difus(fnormal2,L1,colorFocus1);
  vec3 difus2 = Difus(fnormal2,L2,colorFocus2);
  vec3 difus3 = Difus(fnormal2,L3,colorFocus3);
  vec3 difus4 = Difus(fnormal2,L4,colorFocus4);

  vec3 ambient = Ambient();
  vec3 Lambert = ambient + difus + difus1 + difus2 + difus3 + difus4;

  vec3 Phong = Especular(fnormal2,L,fvertex,colorFocus);
  vec3 Phong1 = Especular(fnormal2,L1,fvertex,colorFocus1);
  vec3 Phong2 = Especular(fnormal2,L2,fvertex,colorFocus2);
  vec3 Phong3 = Especular(fnormal2,L3,fvertex,colorFocus3);
  vec3 Phong4 = Especular(fnormal2,L4,fvertex,colorFocus4);

  FragColor = vec4(Lambert + Phong + Phong1 + Phong2 + Phong3 + Phong4,1);
}
