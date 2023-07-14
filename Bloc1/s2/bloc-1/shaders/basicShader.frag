#version 330 core

out vec4 FragColor; //r,g,b,a
in vec3 fcolor;

void main() {
    /*
    ex2:
    if(mod(gl_FragCoord.y,20.) <10.) discard;
    else {
        if(gl_FragCoord.x<354. && gl_FragCoord.y > 354.) FragColor = vec4(1, 0, 0, 1); //rojo
        else if(gl_FragCoord.x>354. && gl_FragCoord.y > 354.) FragColor = vec4(0, 0, 1, 1);//azul
        else if(gl_FragCoord.x<354. && gl_FragCoord.y < 354.) FragColor = vec4(1, 1, 0, 1); //amarillo
        else if(gl_FragCoord.x>354. && gl_FragCoord.y < 354.) FragColor = vec4(0, 1, 0, 1);//verde
    }
    */

    FragColor = vec4(fcolor,1.);

    
}

