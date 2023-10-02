--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

function pass_setup_common (shader, t_base, t_second, t_detail)
	shader	: blend	(false, blend.one, blend.zero)
			: zb	(true,  true)
			: fog	(false)

			:dx10stencil	( 	true, cmp_func.always,
								255 , 127, 
								stencil_op.keep, stencil_op.replace, stencil_op.keep)
			:dx10stencil_ref	(1)

	shader:dx10texture	("s_base", t_base)
	shader:dx10texture	("s_hemi", t_base .. "_nm")

	shader:dx10sampler	("smp_base");
	shader:dx10sampler	("smp_linear");
end

function l_special	(shader, t_base, t_second, t_detail)
	shader	:begin	("lod","lod")
	details_lod.pass_setup_common(shader, t_base, t_second, t_detail)		
end
