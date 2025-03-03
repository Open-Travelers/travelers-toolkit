#pragma once

#include "../../ui/view.h"
#include "../../application_project.h"
#include "../../application_data.h"

class WelcomeView : public UI::View {
protected:
    char m_iso_path[PATH_MAX];

public:
    WelcomeView(ApplicationData &app);
    ~WelcomeView() override;
    ApplicationProject m_project;

protected:
    void on_load(int display_width, int display_height) override;
    UI::ViewChange on_unload() override;

    void on_update(float dt) override;
    void on_render() override;
};
