#include "ui/MainApplication.h"

#if defined(CINDER_MSW_DESKTOP)
#include "windows.h"
#endif

namespace pepr3d {

MainApplication::MainApplication()
    : mToolbar(*this), mSidePane(*this), mModelView(*this), mIntegerManager(mIntegerState) {}

void MainApplication::setup() {
    setWindowSize(950, 570);
    getWindow()->setTitle("Pepr3D");
    setupIcon();

    auto uiOptions = ImGui::Options();
    std::vector<ImWchar> textRange = {0x0001, 0x00BF, 0};
    std::vector<ImWchar> iconsRange = {ICON_MIN_MD, ICON_MAX_MD, 0};
    uiOptions.fonts({make_pair<fs::path, float>(getAssetPath("fonts/SourceSansPro-SemiBold.ttf"), 1.0f * 18.0f),
                     make_pair<fs::path, float>(getAssetPath("fonts/MaterialIcons-Regular.ttf"), 1.0f * 24.0f)},
                    true);
    uiOptions.fontGlyphRanges("SourceSansPro-SemiBold", textRange);
    uiOptions.fontGlyphRanges("MaterialIcons-Regular", iconsRange);
    ImGui::initialize(uiOptions);
    applyLightTheme(ImGui::GetStyle());

    mModelView.setup();

    mGeometry = std::make_unique<Geometry>();
}

void MainApplication::resize() {
    mModelView.resize();
}

void MainApplication::mouseDown(MouseEvent event) {
    // onModelViewMouseDown(mState, event);
}

void MainApplication::mouseDrag(MouseEvent event) {
    // onModelViewMouseDrag(mState, event);
}

void MainApplication::fileDrop(FileDropEvent event) {
    if(mGeometry == nullptr || event.getFiles().size() < 1) {
        return;
    }
    mGeometry->loadNewGeometry(event.getFile(0).string());
}

void MainApplication::update() {}

void MainApplication::draw() {
    gl::clear(ColorA::hex(0xFCFCFC));

    if(mShowDemoWindow) {
        ImGui::ShowDemoWindow();
    }

    // drawToolbar(mState);
    mToolbar.draw();
    mSidePane.draw();
    mModelView.draw();

    ImGui::Begin("##sidepane-debug");
    ImGui::Text((std::string("Geometry is ") + ((mGeometry == nullptr) ? "not loaded" : "loaded")).c_str());
    static int addedValue = 1;
    ImGui::Text("Current value: %i", mIntegerState.mInnerValue);
    if(mIntegerManager.canUndo()) {
        if(ImGui::Button("Undo")) {
            mIntegerManager.undo();
        }
    }
    if(mIntegerManager.canRedo()) {
        if(ImGui::Button("Redo")) {
            mIntegerManager.redo();
        }
    }
    ImGui::SliderInt("##addedvalue", &addedValue, 1, 10);
    ImGui::SameLine();
    if(ImGui::Button("Add")) {
        mIntegerManager.execute(make_unique<AddValueCommand>(addedValue));
    }
    ImGui::End();
}

void MainApplication::setupIcon() {
#if defined(CINDER_MSW_DESKTOP)
    auto dc = getWindow()->getDc();
    auto wnd = WindowFromDC(dc);
    auto icon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(101));  // see resources/Resources.rc
    SendMessage(wnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
    SendMessage(wnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
#endif
}

}  // namespace pepr3d