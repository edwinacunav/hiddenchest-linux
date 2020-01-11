
uniform sampler2D texture;
uniform lowp float gray;
varying vec2 v_texCoord;
const vec3 lumaF = vec3(.299, .587, .114);

void main() {
  vec4 frag = texture2D(texture, v_texCoord);
  float luma = dot(frag.rgb, lumaF);
  frag.rgb = mix(frag.rgb, vec3(luma), gray);
  gl_FragColor = frag;
}
