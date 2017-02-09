in vec3 aPosition;
varying vec4 vColor;
uniform mat4 uMVP;

void main() {
	vColor = vec4(1.0, 1.0, 1.0, 1.0);
  // gl_Position = uMVP * vec4(aPosition, 1.0);
  gl_Position = vec4(aPosition, 1.0);
}
