
uniform sampler2D diffuse;
float seaLevel;

varying float posY;
varying vec2 texCoord;

void main() {

    if(posY<seaLevel) discard;

    gl_FragColor=texture2D(diffuse, texCoord);

}