
uniform lowp vec4 c1;
uniform lowp vec4 c2;
uniform vec2 origin;
uniform vec2 resolution;

void main() {
  vec2 st = gl_FragCoord.xy / resolution.xy;
  float mix_value = distance(st, origin);
  gl_FragColor = mix(c1, c2, mix_value);
}
