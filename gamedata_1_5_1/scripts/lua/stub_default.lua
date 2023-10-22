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
	shader:begin	("stub_default","stub_default")

--	Decouple sampler and texture
--	shader:sampler	("s_base")	: texture(t_base)	: clamp() : f_linear ()
--	TODO: DX10: move stub_default to smp_rtlinear
	shader:dx10texture	("s_base", t_base)
	shader:dx10sampler	("smp_base")
end