#define TESLA_INIT_IMPL
#include <tesla.hpp>

// Wake combo: hold L + R + A. Must differ from your Tesla launch combo.
static constexpr u64 WAKE_COMBO = HidNpadButton_L | HidNpadButton_R | HidNpadButton_A;

// True while BlackGui is on top of the stack. Used to pop it on the next show,
// so reopening the overlay lands on the menu instead of a black screen.
static bool s_screenBlack = false;

// Fullscreen opaque black. On OLED, black pixels are off.
class BlackElement : public tsl::elm::Element {
public:
    void draw(tsl::gfx::Renderer *renderer) override {
        renderer->fillScreen(tsl::Color(0x0, 0x0, 0x0, 0xF));
    }
    void layout(u16, u16, u16, u16) override {}
};

class BlackGui : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override { return new BlackElement(); }

    bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState&,
                     HidAnalogStickState, HidAnalogStickState) override {
        // Require a fresh press so the A that opened this Gui can't wake it.
        if ((keysHeld & WAKE_COMBO) == WAKE_COMBO && (keysDown & WAKE_COMBO)) {
            // hide() fades the overlay out and returns input to the game/app.
            // (close() would exit to Tesla-Menu instead.)
            tsl::Overlay::get()->hide();
        }
        return true; // swallow everything else
    }
};

class MenuGui : public tsl::Gui {
public:
    tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame("ScreenOff", "v0.1.0");
        auto list  = new tsl::elm::List();

        list->addItem(new tsl::elm::CategoryHeader("Press A, wake with L+R+A"));

        auto item = new tsl::elm::ListItem("Screen off");
        item->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                s_screenBlack = true;
                tsl::changeTo<BlackGui>();
                return true;
            }
            return false;
        });
        list->addItem(item);

        frame->setContent(list);
        return frame;
    }
};

class ScreenOffOverlay : public tsl::Overlay {
public:
    // Runs before the overlay becomes visible again (not inside a Gui's input
    // handler), so popping the Gui stack here is safe.
    void onShow() override {
        if (s_screenBlack) {
            s_screenBlack = false;
            tsl::goBack();
        }
    }

    std::unique_ptr<tsl::Gui> loadInitialGui() override { return initially<MenuGui>(); }
};

int main(int argc, char **argv) {
    return tsl::loop<ScreenOffOverlay>(argc, argv);
}
