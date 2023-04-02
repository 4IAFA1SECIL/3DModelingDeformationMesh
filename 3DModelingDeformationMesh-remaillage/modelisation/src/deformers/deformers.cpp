#include "deformers.hpp"


using namespace cgp;

void apply_deformation(mesh& shape, numarray<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters) {

    /**
        shape:  The position of shape are the one to be deformed
        position_before_deformation:  Reference position of the mesh before applying this deformation
        translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
        picked_position:    3D Position of the picked vertex
        picked_normal:      Normal of the surface at the picked vertex position
        camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
    */


    float const r = deformer_parameters.falloff; // radius of influence of the deformation
    size_t const N = shape.position.size();
    for (size_t k = 0; k < N; ++k) {
        vec3 &p_shape = shape.position[k];                             // position to deform
        vec3 const &p_shape_original = position_before_deformation[k]; // reference position before deformation
        vec3 const &p_clicked = picked_position;                       // 3D position of picked position
        vec3 const &n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position

        float const dist = norm(p_clicked -
                                p_shape_original);         // distance between the picked position and the vertex before deformation

        if (deformer_parameters.type == deform_translate) // Case of translation
        {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + (1 - dist / r) * translation;

        }
        if (deformer_parameters.type == deform_translate_square) //deformation using the square of the distance
        {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + translation * (1 - dist * dist / (r * r));

        }
        if (deformer_parameters.type == deform_translate_constante) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + translation;
        }
        if (deformer_parameters.type == deform_translate_carre) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + pow(1 - dist / r, 4) * translation;
        }
        if (deformer_parameters.type == deform_translate_racine) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + sqrt(1 - dist / r) * translation;
        }
        if (deformer_parameters.type == deform_translate_sinus) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + sinh(1 - dist / r) * translation;
        }
        if (deformer_parameters.type == deform_translate_Hugo) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + pow(1 - pow(dist / r, 4), 4) * translation;
        }
        if (deformer_parameters.type == deform_translate_Leo) {
            vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
            if (dist < r)
                p_shape = p_shape_original + (1 - 1 / (1 + 5 * (1 - dist / r))) * translation;
        }
        if (deformer_parameters.type == deform_twist) // Case of twist
        {
            if (dist < r) {
                vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
                float angle;
                if (translation.x >= 0.0f)
                    angle = 2 * (1.0f - dist / r) * norm(translation);
                else
                    angle = -2 * (1.0f - dist / r) * norm(translation);
                vec3 const p_to_p_clicked = p_shape_original - p_clicked;
                rotation_transform const rotation = rotation_transform::from_axis_angle(n_clicked, angle);
                vec3 const p_rotated = rotation * p_to_p_clicked;
                p_shape = p_rotated + p_clicked;
            }

        }
        if (deformer_parameters.type == deform_scale) {

        }

    }
}


