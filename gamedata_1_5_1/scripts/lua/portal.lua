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
--	shader	:begin		("portal","simple_color")
	shader	:begin		("portal","portal")
			:fog		(true)
			:zb 		(true,false)
			:sorting	(3, true)
			:blend		(true,blend.srcalpha,blend.invsrcalpha)
--	TODO: DX10: Setup samplers
--	shader	:sampler	("s_tonemap")   :texture	("$user$tonemap")
	shader:dx10texture	("s_tonemap", "$user$tonemap")
end
