#include "gui.hpp"


bool gui_parameters::display()
{
    ImGui::Checkbox("Display frame", &display_frame);
    ImGui::Checkbox("Wireframe", &display_wireframe);

    // Create interface
    ImGui::Text("Surface type:"); // Select surface to be deformed
    int* ptr_surface_type  = reinterpret_cast<int*>(&surface_type); // trick - use pointer to select enum
    bool new_surface = false;
    new_surface |= ImGui::RadioButton("Plane",ptr_surface_type, surface_plane); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cylinder",ptr_surface_type, surface_cylinder); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Sphere",ptr_surface_type, surface_sphere); ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Cube",ptr_surface_type, surface_cube);  ImGui::SameLine();
    new_surface |= ImGui::RadioButton("Mesh",ptr_surface_type, surface_mesh);


    ImGui::Text("Deformer type:"); // Select the type of deformation to apply
    ImGui::Text("Translate:");
    int* ptr_deformer_type = (int*)&deformer_parameters.type;
    ImGui::RadioButton("Linéaire",ptr_deformer_type, deform_translate); ImGui::SameLine();
    ImGui::RadioButton("Constante",ptr_deformer_type, deform_translate_constante); ImGui::SameLine();
    ImGui::RadioButton("Carré",ptr_deformer_type, deform_translate_carre); ImGui::SameLine();
    ImGui::RadioButton("TranslateSquare",ptr_deformer_type, deform_translate_square); ImGui::SameLine();
    ImGui::RadioButton("Racine",ptr_deformer_type, deform_translate_racine); ImGui::SameLine();
    ImGui::RadioButton("Sinus Hyperbolique",ptr_deformer_type, deform_translate_sinus); ImGui::SameLine();
    ImGui::RadioButton("Hugo",ptr_deformer_type, deform_translate_Hugo); ImGui::SameLine();
    ImGui::RadioButton("Léo",ptr_deformer_type, deform_translate_Leo);
    ImGui::Text("Other:");
    ImGui::RadioButton("Twist",ptr_deformer_type, deform_twist); ImGui::SameLine();
    ImGui::RadioButton("Scale",ptr_deformer_type, deform_scale);

    ImGui::Text("Deformer direction:"); // Select the type of deformation to apply
    int* ptr_deformer_direction = (int*)&deformer_parameters.direction;
    ImGui::RadioButton("View space",ptr_deformer_direction, direction_view_space); ImGui::SameLine();
    ImGui::RadioButton("Surface normal",ptr_deformer_direction, direction_surface_normal);

    // Select falloff distance using slider
    ImGui::SliderFloat("Falloff distance", &deformer_parameters.falloff, 0.01f, 0.8f);

    return new_surface;
}