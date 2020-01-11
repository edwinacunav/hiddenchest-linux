
uniform sampler2D texture;
varying vec2 v_texCoord;

void main() {
  vec4 texColor = texture2D(texture, v_texCoord);
  gl_FragColor = vec4(1.0 - texColor.rgb, 1.0);
}