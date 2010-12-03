
uniform mat4 xWorld;

varying float posY;
varying vec2 texCoord;

void main() {

    gl_Position=ftransform();
    posY=(xWorld*gl_Vertex).y;
    texCoord=gl_MultiTexCoord0;

}