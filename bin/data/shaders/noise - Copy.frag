uniform sampler2DRect baseTex;
uniform sampler2DRect bgTex;
uniform sampler2DRect vignetteTex;
uniform vec2 uNoiseOffset;

// Component wise blending
#define Blend(base, blend, funcf) 	vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b))

#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
#define BlendOverlay(base, blend) 	Blend(base, blend, BlendOverlayf)

void main() {
//	gl_FragColor = vec4(texCoord.s/1280.0, texCoord.t/800.0, 0, 1 );
		
	// FBO of screen
	vec4 col1 = texture2DRect(baseTex, gl_TexCoord[0].xy);	
	// noise texture
	vec4 col2 = texture2DRect(bgTex, gl_TexCoord[0].xy + uNoiseOffset);
	col2.a = 0.2;
	// vignette texture
	vec4 col3 = texture2DRect(vignetteTex, gl_TexCoord[0].xy);
	
	// multiply screen by vignette
	vec3 colFinal = col1.rgb * col3.rgb;
	
	// overlay noise @ noise's opacity
	colFinal = mix(colFinal, BlendOverlay(colFinal, col2), col2.a);
	
	
	gl_FragColor = vec4(colFinal, 1.);
}