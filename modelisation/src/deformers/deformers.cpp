#include "deformers.hpp"

#include <algorithm>

using namespace cgp;

void apply_deformation(mesh& shape, numarray<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters)
{

	/** 
		shape:  The position of shape are the one to be deformed
		position_before_deformation:  Reference position of the mesh before applying this deformation
		translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
		picked_position:    3D Position of the picked vertex
		picked_normal:      Normal of the surface at the picked vertex position
		camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
	*/


	float const r = deformer_parameters.falloff; // radius of influence of the deformation
	float const n = deformer_parameters.n; // exponent for the power function
	float const m = deformer_parameters.m; // exponent for the power function
	float const t = deformer_parameters.t; // Taille du remaillage
	size_t const N = shape.position.size();
	std::vector<size_t> modified;

	for (size_t k = 0; k < N; ++k)
	{
		vec3& p_shape = shape.position[k];                             // position to deform
		vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation
		vec3 const& p_clicked = picked_position;                       // 3D position of picked position
		
		vec3 const& n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position

		float const dist = norm(p_clicked - p_shape_original);         // distance between the picked position and the vertex before deformation

		// TO DO: Implement the deformation models
		// **************************************************************** //
		// ...
		if (deformer_parameters.type == deform_translate) // Case of translation
		{
			// Hint: You can convert the 2D translation in screen space into a 3D translation in the view plane in multiplying 
			//       camera_orientation * vec3(translate_screen, 0)
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);

			// Fake deformation (linear translation in the screen space) 
			//   the following lines should be modified to get the expected smooth deformation
			if (dist < r){
				p_shape = p_shape_original + (1 - dist / r) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}

		}
		if (deformer_parameters.type == deform_translate_constante)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_carre)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + pow(1 - dist / r, 4) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_racine)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + sqrt(1 - dist / r) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_sinus)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + sinh(1 - dist / r) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_Hugo)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + pow(1 - pow(dist / r, 4), 4) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_Leo)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if (dist < r){
				p_shape = p_shape_original + (1-1/(1+5*(1 - dist / r))) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_translate_Loic)
		{
			vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
			if  ((pow(r,n*m))*pow((pow(r,n)-pow(dist,n)),m) == 0){
				std::cout << "Division par 0" << std::endl;
				break;
			}else if (dist < r){
				p_shape = p_shape_original + (1/(pow(r,n*m))*pow((pow(r,n)-pow(dist,n)),m)) * translation;
				if (deformer_parameters.remaillage){
					modified.push_back(k);
				}
			}
		}
		if (deformer_parameters.type == deform_twist)
		{
			// Deformation to implement"
			if (deformer_parameters.remaillage){
					modified.push_back(k);
			}
		}

	}
	if (deformer_parameters.remaillage){
		for (size_t i = 0; i < modified.size(); i++)
		{
			size_t const N = shape.connectivity.size();
			std::set <int> voisins;
			for (size_t k = 0; k < N; ++k)
			{
				uint3 const& tri = shape.connectivity[k];
				
				if (get<0>(tri) == modified[i] || get<1>(tri) == modified[i] || get<2>(tri) == modified[i]){
					voisins.insert(get<0>(tri));
					voisins.insert(get<1>(tri));
					voisins.insert(get<2>(tri));
				}
			}
			// Récupération de la position du sommet i
			vec3 vertex_pos = shape.position[modified[i]];
			// Parcours des sommets voisins
			for (size_t j = 0; j < voisins.size(); j++)
			{
				// Récupération de la position du sommet j
				auto it = voisins.begin();
				std::advance(it, j);

				vec3 vertex_pos_neighbour = shape.position[*it];

				// Calcul de la distance entre les deux sommets
				float dist = norm(vertex_pos - vertex_pos_neighbour);
				
				if (dist > t)
				{
					vec3 new_vertex_pos = (vertex_pos + vertex_pos_neighbour) / 2.0f;
					int index_new_pos = shape.position.size();
					std::vector<int> temps = {};
					std::vector<int> ks = {};
					for (size_t k = 0; k < shape.connectivity.size(); k++)
					{
						bool found1 = false;
						bool found2 = false;
						int temp;
						for (size_t l = 0; l < shape.connectivity[k].size(); l++)
						{
							if (shape.connectivity[k][l] == *it)
							{
								found1 = true;
							}
							else if (shape.connectivity[k][l] == modified[0])
							{
								found2 = true;
							}
							else {
								temp = shape.connectivity[k][l];
							}
						}
						if (found1 && found2)
						{
							temps.push_back(temp);
							ks.push_back(k);
						}
					}
					
					for ( int k = 0; k < ks.size(); k++)
					{
						std::vector<uint3>& connectivity_vector = reinterpret_cast<std::vector<uint3>&>(shape.connectivity);
						connectivity_vector.erase(connectivity_vector.begin() + (ks[k]-k));
						shape.connectivity = reinterpret_cast<numarray<uint3>&>(connectivity_vector);
						shape.connectivity.push_back( uint3{modified[0], temps[k], index_new_pos} );
						shape.connectivity.push_back( uint3{*it, temps[k], index_new_pos} );
					}
					shape.position.push_back(new_vertex_pos);
					vec3 normalb = normalize(new_vertex_pos);
					vec3 normal = {0.5f, 0.5f, 0.5f};
					shape.normal.push_back(normal);
					vec3 color = {1.0f, 1.0f, 1.0f};
					shape.color.push_back(color);
					vec2 uv = {0.0f, 0.0f};
					shape.uv.push_back(uv);
				}
			}
		}
	}

}