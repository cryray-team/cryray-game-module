--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

function normal    (shader, t_base, t_second, t_detail)
  shader:begin  ("model_distort4glass","particle_hard")
      : sorting  (2,true)
      : blend    (true,blend.srcalpha,blend.invsrcalpha)
      : aref     (true,0)
      : zb     (true,true)
      : fog    (false)
      : distort   (false)
--  shader:sampler  ("s_base")      :texture  (t_base)
	shader: dx10texture ("s_base", t_base)
	shader: dx10sampler ("smp_base")
end

function l_special  (shader, t_base, t_second, t_detail)
  shader:begin  ("model_distort4glass","particle_distort")
      : sorting  (2, true)
      : blend    (true,blend.srcalpha,blend.invsrcalpha)
      : zb     (true,true)
      : fog    (false)
      : distort   (true)
--  shader:sampler  ("s_base")      :texture  (t_base)
--  shader:sampler  ("s_distort")   :texture  ("pfx\\pfx_dist_glass3")
	shader: dx10texture ("s_base", t_base)
	shader: dx10texture ("s_distort", "pfx\\pfx_dist_glass3") --:texture  (t_base) -- ("pfx\\pfx_dist_glass2")
	shader: dx10sampler ("smp_linear")
end
