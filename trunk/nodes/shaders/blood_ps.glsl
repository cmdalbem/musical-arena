
uniform float time;
uniform sampler2D blood;

varying vec2 texCoord;

void main() {

    time/=100.0;
    texCoord.x+=sin(25.0*texCoord.x+time)/40.0;
    texCoord.y-=sin(25.0*texCoord.y+time)/40.0;

    gl_FragColor=texture2D(blood, texCoord);

}