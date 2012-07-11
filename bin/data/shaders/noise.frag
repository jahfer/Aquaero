uniform sampler2DRect baseTex;
uniform sampler2DRect bgTex;
uniform sampler2DRect vignetteTex;
uniform vec2 uNoiseOffset;
uniform bool uUseAberration;
uniform float uAberrationOffset;

// Component wise blending
#define Blend(base, blend, funcf) 	vec3(funcf(base.r, blend.r), funcf(base.g, blend.g), funcf(base.b, blend.b))

#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
#define BlendOverlay(base, blend) 	Blend(base, blend, BlendOverlayf)

void main() {
//	gl_FragColor = vec4(texCoord.s/1280.0, texCoord.t/800.0, 0, 1 );
		
	vec3 rCol 	= vec3(1, 0, 0);
	vec3 gCol 	= vec3(0, 1, 0);
	vec3 bCol 	= vec3(0, 0, 1);		
	vec2 coord = gl_TexCoord[0].xy;
	
	//bool useAberration = uUseAberration > 0;
	
	// FBO of screen
	if (uUseAberration)
		coord.x -= uAberrationOffset; 	// offset red
	vec4 fbo1 	= texture2DRect(baseTex, coord);		
	
	if (uUseAberration)
		coord.x += uAberrationOffset; 	// center green
	vec4 fbo2 	= texture2DRect(baseTex, gl_TexCoord[0].xy);	
	
	if (uUseAberration)
		coord.x += uAberrationOffset;	 // offset blue		
	vec4 fbo3 	= texture2DRect(baseTex, coord);	
	
	// FBO channels mixed (incl. offsets)
	vec3 col1 = vec3(fbo1.r, fbo2.g, fbo3.b);
	
	// Noise texture
	vec4 col2 = texture2DRect(bgTex, gl_TexCoord[0].xy + uNoiseOffset);
	col2.a = 0.2;
	
	// Vignette texture
	vec4 col3 = texture2DRect(vignetteTex, gl_TexCoord[0].xy);
	
	// Multiply screen by vignette
	vec3 colFinal = col1.rgb * col3.rgb;
	
	// Overlay noise @ noise's opacity
	colFinal = mix(colFinal, BlendOverlay(colFinal, col2), col2.a);
	
	// Output final pixel color
	gl_FragColor = vec4(colFinal, 1.0);
}