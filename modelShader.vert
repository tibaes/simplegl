in vec3 aPosition;
in vec3 aNormal;

varying vec4 vColor;
varying vec3 vNormal;

uniform mat4 uMVP;
uniform mat3 uNormalMat;

void main() {
	vColor = vec4(1.0, 1.0, 1.0, 1.0);
	vNormal = normalize(aNormal * uNormalMat);
  gl_Position = uMVP * vec4(aPosition, 1.0);
}
