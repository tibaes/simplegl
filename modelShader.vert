#version 330 core

in vec3 aPosition;
in vec3 aNormal;

out vec3 vNormal;
out vec2 vUV;

uniform mat4 uMVP;
uniform mat3 uNormalMat;

void main() {
	vNormal = normalize(aNormal * uNormalMat);
  gl_Position = uMVP * vec4(aPosition, 1.0);
	vUV = vec2(aPosition.x, aPosition.y);
}
