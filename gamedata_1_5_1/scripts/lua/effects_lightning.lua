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
	shader	: begin	("stub_default","stub_default")
			: zb	(true,false)
			: blend	(true,blend.srcalpha,blend.one)
--	TODO: DX10: implement aref for this shader
			: aref 		(true,2)

	shader 	: dx10texture	("s_base", t_base)
--	TODO: DX10: Clamp
	shader 	: dx10sampler	("smp_base") --	Clamp
end