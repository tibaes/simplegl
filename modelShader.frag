varying vec4 vColor;
varying vec3 vNormal;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strenght;

void main() {
  float difuse = max(0.0, dot(vNormal, LightDirection));
  float specular = max(0.0, dot(vNormal, HalfVector));

  if (difuse == 0.0)
    specular = 0.0;
  else
    specular = pow(specular, Shininess);

  vec3 scatteredLight = Ambient + LightColor * difuse;
  vec3 reflectedLight = LightColor * specular *  Strenght;

  vec3 rgb = min(vColor.rgb * scatteredLight + reflectedLight, vec3(1.0));
  gl_FragColor = vec4(rgb, vColor.a);
}
