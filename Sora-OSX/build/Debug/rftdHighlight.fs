struct output {
  float4 color : COLOR;
};

uniform float4 hlColor;

output highlight(float2 texCoord: TEXCOORD0,
                  uniform sampler2D decals: TEX0)
{
  output OUT;
  float4 texcolor = tex2D(decals, texCoord);
  float4 ratecolor = texcolor * hlColor;
  
  OUT.color = texcolor * 0.2 + ratecolor * 0.8;
  
  return OUT;
}

