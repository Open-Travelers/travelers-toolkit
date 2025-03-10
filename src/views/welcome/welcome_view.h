#pragma once

#include "../../ui/view.h"
#include "../../application_project.h"
#include "../../application_data.h"
#include "twoc/nu/scene.h"
#include "scene_renderer.h"
#include "camera.h"

class WelcomeView : public UI::View {
protected:
    Twoc::Nu::Scene *m_scene { nullptr };
    Twoc::Nu::Scene *m_crate_scene { nullptr };
    Twoc::Nu::Scene *m_wumpa_scene { nullptr };
    SceneRenderer m_renderer;
    Camera m_camera;
    glm::mat4 m_projection_matrix;

    std::vector<std::string> m_texture_names;
    int m_texture_selected { -1 };

    std::string const& texture_name(int index) const { return m_texture_names[index]; }
    bool load_scenes();
    bool load_scene(Twoc::Nu::Scene *scene);

    void on_load(int display_width, int display_height) override;
    UI::ViewChange on_unload() override;

    void on_resize(int width, int height) override;
    void on_update(float dt) override;
    void on_render() override;

public:
    WelcomeView(ApplicationData &app);
    ~WelcomeView() override;

};
