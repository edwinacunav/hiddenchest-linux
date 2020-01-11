
uniform mat4 transform;
uniform mat4 projMat;
attribute vec4 position;
attribute vec4 v_color;
attribute vec2 texCoord;
varying vec4 color;
varying vec4 v_texCoord;

void main() {
  gl_Position = transform * position;
  color = v_color;
  v_texCoord = projMat * vec4(texCoord, 1.0, 1.0);
}
