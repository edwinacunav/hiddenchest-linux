varying vec3 light_dir, normal;
varying vec4 v_texCoord;

void main()
{
  normal = normalize(gl_NormalMatrix * gl_Normal);
  light_dir = normalize(vec3(gl_LightSource[0].position));
  v_texCoord = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
