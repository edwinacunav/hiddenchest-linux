
uniform mat4 projMat;
attribute vec2 position;
attribute vec2 texCoord;
attribute vec4 color;
varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{
  v_color = color;
  v_texCoord = texCoord;
  gl_Position = projMat * vec4(position, 0.0, 1.0);
}