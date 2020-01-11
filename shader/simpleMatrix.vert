
uniform mat4 projMat;
uniform mat4 matrix;
uniform vec2 texSizeInv;
attribute vec2 position;
attribute vec2 texCoord;
attribute lowp vec4 color;
varying vec2 v_texCoord;
varying lowp vec4 v_color;

void main()
{
  v_texCoord = texCoord * texSizeInv;
  v_color = color;
  gl_Position = projMat * matrix * vec4(position, 0, 1);
}
