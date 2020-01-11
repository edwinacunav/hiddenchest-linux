
uniform sampler2D texture;
uniform lowp float opacity;
uniform lowp vec4 c1;
uniform lowp vec4 c2;
uniform vec2 origin;
uniform vec2 resolution;
varying vec2 v_texCoord;

void main() {
  vec4 frag = texture2D(texture, v_texCoord);
  vec2 st = gl_FragCoord.xy / resolution.xy;
  float mix_value = distance(st, origin);
  vec4 grad = mix(c1, c2, mix_value);
  gl_FragColor = mix(frag, grad, opacity);
}
