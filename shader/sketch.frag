varying highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform mediump float intensity;
uniform mediump float imageWidthFactor;
uniform mediump float imageHeightFactor;
const mediump vec3 W = vec3(0.2125, 0.7154, 0.0721);

void main()
{
  mediump vec3 textureColor = texture2D(inputImageTexture, textureCoordinate).rgb;
  mediump vec2 stp0 = vec2(1.0 / imageWidthFactor, 0.0);
  mediump vec2 st0p = vec2(0.0, 1.0 / imageHeightFactor);
  mediump vec2 stpp = vec2(1.0 / imageWidthFactor, 1.0 / imageHeightFactor);
  mediump vec2 stpm = vec2(1.0 / imageWidthFactor, -1.0 / imageHeightFactor);
  mediump float i00   = dot( textureColor, W);
  mediump float im1m1 = dot( texture2D(inputImageTexture, textureCoordinate - stpp).rgb, W);
  mediump float ip1p1 = dot( texture2D(inputImageTexture, textureCoordinate + stpp).rgb, W);
  mediump float im1p1 = dot( texture2D(inputImageTexture, textureCoordinate - stpm).rgb, W);
  mediump float ip1m1 = dot( texture2D(inputImageTexture, textureCoordinate + stpm).rgb, W);
  mediump float im10 = dot( texture2D(inputImageTexture, textureCoordinate - stp0).rgb, W);
  mediump float ip10 = dot( texture2D(inputImageTexture, textureCoordinate + stp0).rgb, W);
  mediump float i0m1 = dot( texture2D(inputImageTexture, textureCoordinate - st0p).rgb, W);
  mediump float i0p1 = dot( texture2D(inputImageTexture, textureCoordinate + st0p).rgb, W);
  mediump float h = -im1p1 - 2.0 * i0p1 - ip1p1 + im1m1 + 2.0 * i0m1 + ip1m1;
  mediump float v = -im1m1 - 2.0 * im10 - im1p1 + ip1m1 + 2.0 * ip10 + ip1p1;
  mediump float mag = 1.0 - length(vec2(h, v));
  mediump vec3 target = vec3(mag);
  gl_FragColor = vec4(mix(textureColor, target, intensity), 1.0);
}