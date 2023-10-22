--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

function normal		(shader, t_base, t_second, t_detail)
	shader:begin	("accum_volumetric", "accum_volumetric")
			: fog		(false)
			: zb 		(true,false)
			: blend		(true,blend.one,blend.one)
			: sorting	(2, false)

	shader:dx10texture	("s_lmap", t_base)	
	shader:dx10texture	("s_smap", "null")
	shader:dx10texture	("s_noise", "fx\\fx_noise")

	shader:dx10sampler	("smp_rtlinear")
	shader:dx10sampler	("smp_linear")
	shader:dx10sampler	("smp_smap")
end