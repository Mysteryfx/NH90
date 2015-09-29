local my_path = LockOn_Options.script_path.."SimpleHUD/Lua/"

dofile(my_path.."HUD_definitions.lua")

local mask_material_TFOV 			  = MakeMaterial(nil,{255,153,51,30}) -- orange

total_field_of_view	 				  = CreateElement "ceMeshPoly" -- untextured shape
total_field_of_view.name 			  = "TFOV"
total_field_of_view.material		  = mask_material_TFOV
total_field_of_view.h_clip_relation   = h_clip_relations.INCREASE_IF_LEVEL  -- check clipping : pixel on glass then increase level from GLASS_LEVEL to GLASS_LEVEL+1 = HUD_DEFAULT_LEVEL
total_field_of_view.level			  = GLASS_LEVEL
total_field_of_view.collimated	  	  = true   --
total_field_of_view.isvisible		  = true  -- by default TFOV is not visible and rendered only to stencil buffer
--TFOV is usuall y circle so :
local TFOV_radius  =  120 -- let it be 120 mils

num_points = 32
step = math.rad(360.0/num_points)

local verts = {}
for i = 1, num_points do
	verts[i] = {TFOV_radius * math.cos(i * step), TFOV_radius * math.sin(i * step)}
end

local inds = {}
j = 0
for i = 0, 29 do
	j = j + 1
	inds[j] = 0
	j = j + 1
	inds[j] = i + 1
	j = j + 1
	inds[j] = i + 2
end 

total_field_of_view.vertices = verts
total_field_of_view.indices  = inds
Add(total_field_of_view)


