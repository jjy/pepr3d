#pragma once
#include <memory>
#include "tools/Tool.h"
#include "ui/IconsMaterialDesign.h"
#include "ui/SidePane.h"

#include <optional>
#include "commands/CommandManager.h"
#include "commands/ExampleCommand.h"

namespace pepr3d {

class MainApplication;

/// Tool used for real-time debugging purposes
class LiveDebug : public Tool {
   public:
    explicit LiveDebug(MainApplication& app) : mApplication(app), mIntegerManager(mIntegerState) {}

    virtual std::string getName() const override {
        return "Live Debug";
    }

    virtual std::optional<Hotkey> getHotkey(const Hotkeys& hotkeys) const override {
        return hotkeys.findHotkey(HotkeyAction::SelectLiveDebug);
    }

    virtual std::string getIcon() const override {
        return ICON_MD_DEVELOPER_BOARD;
    }

    virtual void drawToSidePane(SidePane& sidePane) override;
    virtual void drawToModelView(ModelView& modelView) override;

    virtual void onModelViewMouseMove(ModelView& modelView, ci::app::MouseEvent event) override;

   private:
    MainApplication& mApplication;
    IntegerState mIntegerState;
    CommandManager<IntegerState> mIntegerManager;
    glm::ivec2 mMousePos;
    std::optional<std::size_t> mTriangleUnderRay{};

    std::vector<std::size_t> mTrianglesInRadius;

    float mSquaredRadius = 0.1f;
};

}  // namespace pepr3d
