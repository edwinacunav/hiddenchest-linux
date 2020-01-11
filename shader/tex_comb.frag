uniform sampler2D texture;
varying vec3 light_dir, normal;
varying vec4 v_texCoord;

void main()
{
  //vec3 ct, cf;
  vec4 color;//, texel;
  float intensity;//, at, af;
  vec3 light_dir = normalize(vec3(gl_LightSource[0].position));
  intensity = dot(light_dir, gl_Normal);
  if (intensity > 0.95)
    color = vec4(1.0,0.5,0.5,1.0);
  else if (intensity > 0.5)
    color = vec4(0.6,0.3,0.3,1.0);
  else if (intensity > 0.25)
    color = vec4(0.4,0.2,0.2,1.0);
  else
    color = vec4(0.2,0.1,0.1,1.0);
  gl_FragColor = color;
  //intensity = max(dot(light_dir, normalize(normal)), 0.0);
  /*cf = intensity * (gl_FrontMaterial.diffuse).rgb + gl_FrontMaterial.ambient.rgb;
  af = gl_FrontMaterial.diffuse.a;
  texel = texture2D(texture, v_texCoord.st);
  ct = texel.rgb;
  at = texel.a;
  gl_FragColor = vec4(ct * cf, at * af);*/
}
