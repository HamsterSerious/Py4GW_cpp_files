#pragma once
#include "Headers.h"

enum class FontID : int {
	Regular_14,
	Regular_22,
	Regular_30,
	Regular_46,
	Regular_62,
    Regular_124,
	Bold_14,
	Bold_22,
	Bold_30,
	Bold_46,
	Bold_62,
	Bold_124,
	Italic_14,
	Italic_22,
	Italic_30,
	Italic_46,
	Italic_62,
	Italic_124,
	BoldItalic_14,
	BoldItalic_22,
	BoldItalic_30,
	BoldItalic_46,
	BoldItalic_62,
	BoldItalic_124,

	Count
};

class FontManager {
public:
    static FontManager& Instance() {
        static FontManager instance;
        return instance;
    }

    void SetFontDirectory(const std::string& directory) {
        font_dir = directory;
    }

    ImFont* Get(int id) {
        int index = id;
        if (!fonts[index])
			fonts[index] = LoadFont(static_cast<FontID>(id));
        return fonts[index];
    }

    void LoadFonts() {
        for (int i = 0; i < static_cast<int>(FontID::Count); ++i) {
            if (!fonts[i]) {
                fonts[i] = LoadFont(static_cast<FontID>(i));
            }
        }
    }

private:
    std::string font_dir = "";
    std::array<ImFont*, static_cast<int>(FontID::Count)> fonts = { nullptr };

    FontManager() = default;
    ~FontManager() = default;
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    ImFont* LoadFont(FontID id) {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.OversampleH = 1;
        config.OversampleV = 1;
        config.PixelSnapH = true;

        std::string path;
        float size = 14.0f;

        switch (id) {
        case FontID::Regular_14:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 14.0f; break;
        case FontID::Regular_22:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 22.0f; break;
        case FontID::Regular_30:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 30.0f; break;
        case FontID::Regular_46:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 46.0f; break;
        case FontID::Regular_62:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 62.0f; break;
        case FontID::Regular_124:
            path = font_dir + "\\friz-quadrata-std-medium-5870338ec7ef8.otf"; size = 124.0f; break;


        case FontID::Bold_14:
            path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 14.0f; break;
		case FontID::Bold_22:
			path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 22.0f; break;
		case FontID::Bold_30:
			path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 30.0f; break;
		case FontID::Bold_46:
			path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 46.0f; break;
		case FontID::Bold_62:
			path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 62.0f; break;
		case FontID::Bold_124:
			path = font_dir + "\\friz-quadrata-std-bold-587034a220f9f.otf"; size = 124.0f; break;


        case FontID::Italic_14:
            path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 14.0f; break;
		case FontID::Italic_22:
			path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 22.0f; break;
		case FontID::Italic_30:
			path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 30.0f; break;
		case FontID::Italic_46:
			path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 46.0f; break;
		case FontID::Italic_62:
			path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 62.0f; break;
		case FontID::Italic_124:
			path = font_dir + "\\friz-quadrata-std-italic-587033b2c95df.otf"; size = 124.0f; break;
        

        case FontID::BoldItalic_14:
            path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 14.0f; break;
		case FontID::BoldItalic_22:
			path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 22.0f; break;
		case FontID::BoldItalic_30:
			path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 30.0f; break;
		case FontID::BoldItalic_46:
			path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 46.0f; break;
		case FontID::BoldItalic_62:
			path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 62.0f; break;
		case FontID::BoldItalic_124:
			path = font_dir + "\\friz-quadrata-std-bold_italic-587033d6d4298.otf"; size = 124.0f; break;
        

        default:
            assert(false && "Unknown FontID");
            return nullptr;
        }

        ImFont* loaded = io.Fonts->AddFontFromFileTTF(path.c_str(), size, &config);
        assert(loaded != nullptr && "Font loading failed");
        return loaded;
    }
};