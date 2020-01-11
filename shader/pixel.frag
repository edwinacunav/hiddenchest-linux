
uniform sampler2D texture;
varying vec4 color;
varying vec4 v_texCoord;

void main() {
  float px = 5.0;
  int si = int(v_texCoord.s * px);
  int sj = int(v_texCoord.t * px);
  gl_FragColor = texture2D(texture, vec2(float(si) / px, float(sj) / px)) * color;
}
