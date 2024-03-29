#pragma once


#include "cgp/cgp.hpp"


enum deformer_type_enum {
	deform_translate,
	deform_translate_constante,
	deform_translate_carre,
	deform_translate_racine,
	deform_translate_sinus,
	deform_translate_Hugo,
	deform_translate_Leo,
	deform_translate_Loic,
	deform_twist,
	deform_scale
};

enum deformer_direction_enum {
	direction_view_space,
	direction_surface_normal
};

struct deformer_parameters_structure {
	deformer_type_enum type           = deform_translate;      // Type of deformation type
	deformer_direction_enum direction = direction_view_space;  // Type of deformation direction
	float falloff                     = 1 / 5.0f;              // Falloff/influence distance (can be adjusted from the GUI or with mouse scroll)
	float n 						  = 4;                     // Exponent for the power function
	float m 						  = 4;                     // Exponent for the power function
	bool remaillage                   = false;                 // Remaillage
	float t 						  = 2.5;                     // Taille du remaillage
};



void apply_deformation(cgp::mesh& shape, cgp::numarray<cgp::vec3> const& position_before_deformation, cgp::vec2 const& translate_screen, cgp::vec3 const& picked_position, cgp::vec3 const& picked_normal, cgp::rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters);