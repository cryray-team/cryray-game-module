--' //////////////////////////////////////////////////////////////////////////
--' //-' CryRay Engine x64 Shaders
--' //-' Based on shaders from the original Anomaly 1.5.1
--' //-' As well as an integrated shader pack Screen Space 15.4
--' //-' Repository with shaders: https://github.com/cryray-team
--' //-' Also, if you are a shader developer
--' //-' You can join our server in discord: https://discord.gg/R78uVUZCTR
--' //-' OldSerpskiStalker7777, CryRay Team
--' //////////////////////////////////////////////////////////////////////////

local tex_base                = "water\\water_ryaska1"
local tex_nmap0               = "shaders\\water_normal"
local tex_dist                = "water\\water_dudv"
local tex_env0                = "$user$sky0"
local tex_sky0                = "$user$sky0"
local tex_env1                = "$user$sky1"
local tex_sky1                = "$user$sky1"
local tex_leaves              = "water\\water_foam"
local tex_bluenoise           = "shaders\\blue_noise"
local tex_rainsplash          = "shaders\\water_sbumpvolume"
local tex_caustics 	     	  = "shaders\\water_caustics"

function normal                (shader, t_base, t_second, t_detail)
	shader	:begin		("water_ryaska","water_ryaska")
    		:sorting	(2, false)
			:blend		(true,blend.srcalpha,blend.invsrcalpha)
			:zb			(true,false)
			:distort	(true)
			:fog		(true)

	shader:dx10texture	("s_base",		tex_base)
	shader:dx10texture	("s_nmap0",		tex_nmap0)
	shader:dx10texture	("s_env0",		tex_env0)
	shader:dx10texture	("s_env1",		tex_env1)
	shader:dx10texture	("sky_s0",		tex_sky0)
	shader:dx10texture	("sky_s1",		tex_sky1)
	shader:dx10texture	("s_position",	"$user$position")

	shader:dx10texture	("s_leaves",	tex_leaves)
	
	shader:dx10texture  ("s_rimage", "$user$generic_temp")
	shader:dx10texture  ("s_diffuse", "$user$albedo")
	shader:dx10texture  ("s_accumulator", "$user$accum")

	shader:dx10texture  ("s_bluenoise", tex_bluenoise)
	shader:dx10texture  ("s_rainsplash", tex_rainsplash)
	shader:dx10texture  ("s_watercaustics", tex_caustics)	
	
	shader:dx10sampler	("smp_base")
	shader:dx10sampler	("smp_linear")
	shader:dx10sampler	("smp_nofilter")
	shader:dx10sampler	("smp_rtlinear")
end

function l_special        (shader, t_base, t_second, t_detail)
	shader	:begin                ("waterd","waterd")
			:sorting        (2, true)
			:blend                (true,blend.srcalpha,blend.invsrcalpha)
			:zb                (true,false)
			:fog                (false)
			:distort        (true)

	shader: dx10color_write_enable( true, true, true, false)

	shader:dx10texture	("s_base",		tex_base)
	shader:dx10texture	("s_distort",	tex_dist)

	shader:dx10sampler	("smp_base")
	shader:dx10sampler	("smp_nofilter")	
end
