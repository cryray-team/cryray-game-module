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
	shader:begin	("wmark",	"simple")
			: sorting	(1, false)
			: blend		(true,blend.destcolor,blend.srccolor)
			: aref 		(true,0)
			: zb 		(true,false)
			: fog		(false)
			: wmark		(true)
--	shader:sampler	("s_base")      :texture	(t_base)
	shader: dx10texture ("s_base", t_base)
	shader: dx10sampler ("smp_rtlinear")
	shader: dx10color_write_enable( true, true, true, false)
end
