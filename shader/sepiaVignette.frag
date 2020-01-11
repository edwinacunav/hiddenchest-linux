
uniform sampler2D texture;
uniform vec2 texSizeInv;
varying vec2 v_texCoord;
const float RADIUS = 0.75;
const float SOFTNESS = 0.45;
const vec3 GRAY = vec3(0.299, 0.587, 0.114);
const vec3 SEPIA = vec3(1.2, 1.0, 0.8);

void main() {
  vec4 texColor = texture2D(texture, v_texCoord);
  vec2 pos = (gl_FragCoord.xy / texSizeInv.xy) - vec2(0.5);
  float len = length(pos);
  float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);
  texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.5);
  float gray = dot(texColor.rgb, GRAY);
  vec3 sepiaColor = vec3(gray) * SEPIA;
  texColor.rgb = mix(texColor.rgb, sepiaColor, 1.0);//0.75);
  gl_FragColor = texColor;
}