#version 330 core

in vec3 vNormal;
in vec2 vUV;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector;
uniform float Shininess;
uniform float Strenght;
uniform sampler2D TextureSampler;

out vec3 color;

void main() {
  vec3 texColor = texture(TextureSampler, vUV).rgb;

  float difuse = max(0.0, dot(vNormal, LightDirection));
  float specular = max(0.0, dot(vNormal, HalfVector));

  if (difuse == 0.0)
    specular = 0.0;
  else
    specular = pow(specular, Shininess);

  vec3 scatteredLight = Ambient + LightColor * difuse;
  vec3 reflectedLight = LightColor * specular *  Strenght;

  color = min(texColor * scatteredLight + reflectedLight, vec3(1.0));
}
