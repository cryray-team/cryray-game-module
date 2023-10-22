--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

function normal(shader, t_base, t_second, t_detail)
  shader:begin("effects_sun","stub_default")
--        : sorting        (2, false)
--        : blend                (true,blend.srcalpha,blend.invsrcalpha)
--        : blend                (true,blend.one,blend.one)
        : blend                (true,blend.srcalpha,blend.one)
		: zb                (false,false)
--  shader:sampler        ("s_base")       :texture  (t_base)
	shader:dx10texture	("s_base", t_base)
	shader:dx10sampler	("smp_base")
end