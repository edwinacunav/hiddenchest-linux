
uniform sampler2D texture;
uniform vec2 resolution;
varying vec2 v_texCoord;

void main()
{
  vec4 color = texture2D(texture, v_texCoord);
  vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5);
  float dist = length(position);
  float radius = 0.5;
  float softness = 0.02;
  float vignette = smoothstep(radius, radius - softness, dist);
  color.rgb = color.rgb - (1.0 - vignette);
  gl_FragColor = color;
}