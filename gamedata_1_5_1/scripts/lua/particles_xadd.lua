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
	shader:begin	("particle",	"particle")
			: sorting	(3, false)
			: blend		(true,blend.one,blend.one)
			: aref 		(false,0)
			: zb 		(true,false)
			: fog		(false)
			: distort 	(true)
--	shader:sampler	("s_base")      :texture	(t_base)
--	shader:sampler  ("s_position")  :texture  ("$user$position")

	shader:dx10texture	("s_base"		,t_base)
	shader:dx10texture  ("s_position"	,"$user$position")

	shader:dx10sampler	("smp_base")
	shader:dx10sampler	("smp_nofilter")
end

function l_special	(shader, t_base, t_second, t_detail)
	shader:begin	("particle",	"particle_distort")
			: sorting	(3, false)
			: blend		(true,blend.srcalpha,blend.invsrcalpha)
			: zb 		(true,false)
			: fog		(false)
			: distort 	(true)
--	shader:sampler	("s_base")      :texture	(t_base)
--	shader:sampler	("s_distort")   :texture	(t_second)	-- "pfx\\pfx_distortion"
--	shader:sampler  ("s_position")  :texture  ("$user$position")

	shader:dx10texture	("s_base"		,t_base)
	shader:dx10texture	("s_distort"	,t_second)	-- "pfx\\pfx_distortion"
	shader:dx10texture  ("s_position"	,"$user$position")

	shader:dx10sampler	("smp_base")
	shader: dx10sampler ("smp_linear")
	shader:dx10sampler	("smp_nofilter")
end