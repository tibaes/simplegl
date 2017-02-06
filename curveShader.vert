in vec2 aPosition;
in vec3 aColor;

varying vec4 vColor;

uniform mat4 uMVP;

void main() {
	vColor = vec4(aColor, 1.0);
  //gl_Position = uMVP * vec4(aPosition, 1.0, 1.0);
	gl_Position = vec4(aPosition, 1.0, 1.0);
}
