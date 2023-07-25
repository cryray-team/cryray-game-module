--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

function normal   (shader, t_base, t_second, t_detail)
  shader:begin    ("model_env_lq","model_env_lq")
      : fog       (true)
      : zb        (true,false)
      : blend     (true,blend.srcalpha,blend.invsrcalpha)
      : aref      (true,0)
      : sorting   (3,true)
--  shader:sampler  ("s_base")       :texture    (t_base)
--  shader:sampler  ("s_env")        :texture    ("sky\\sky_5_cube") : clamp()
	shader: dx10texture ("s_base", t_base)
	shader:	dx10texture ("s_env", "sky\\sky_5_cube")

	shader: dx10sampler ("smp_base")
	shader: dx10sampler ("smp_rtlinear")
end

function l_special  (shader, t_base, t_second, t_detail)
  shader:begin  ("model_distort4glass","particle_distort")
      : sorting   (3, true)
      : blend     (true,blend.srcalpha,blend.invsrcalpha)
      : zb        (true,false)
      : fog       (false)
      : distort   (true)
--  shader:sampler  ("s_base")      :texture  (t_base)
--  shader:sampler  ("s_distort")   :texture  ("pfx\\pfx_dist_glass4")
	shader: dx10texture ("s_base", t_base)
	shader: dx10texture ("s_distort", "pfx\\pfx_dist_glass4")
	shader: dx10sampler ("smp_linear")
end

--[[
function normal_hq(shader, t_base, t_second, t_detail)
  shader:begin    ("model_env_hq","model_env_hq")
      : fog       (true)
      : zb        (true,false)
      : blend     (true,blend.srcalpha,blend.invsrcalpha)
      : aref      (true,0)
      : sorting   (3,true)
  shader:sampler  ("s_base")       :texture    (t_base)
  shader:sampler  ("s_env")        :texture    ("sky\\sky_5_cube") : clamp()
  shader:sampler  ("s_lmap")       :texture  ("$user$projector")
    : clamp    ()
    : f_linear   ()
    : project     (true)
end
]]
