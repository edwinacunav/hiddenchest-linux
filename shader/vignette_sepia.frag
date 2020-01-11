
uniform sampler2D texture;
uniform vec2 resolution;
varying vec2 v_texCoord;
const vec3 GRAY = vec3(0.299, 0.587, 0.114);
const vec3 SEPIA = vec3(1.2, 1.0, 0.8);

void main()
{
  vec4 color = texture2D(texture, v_texCoord);
  float gray = dot(color.rgb, GRAY);
  vec3 sepia = vec3(gray) * SEPIA;
  color.rgb = mix(color.rgb, sepia, 0.8);
  vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5);
  float dist = length(position);
  float radius = 0.5;
  float softness = 0.02;
  float vignette = smoothstep(radius, radius - softness, dist);
  color.rgb = color.rgb - (1.0 - vignette);
  gl_FragColor = color;
}