varying vec4 vColor;

uniform vec4 Ambient;

void main() {
  vec4 scatteredLight = Ambient;
  gl_FragColor = min(vColor * scatteredLight, vec4(1.0));
}
