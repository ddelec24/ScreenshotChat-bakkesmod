#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

#include <atlbase.h>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr const char* DEFAULT_SCREENSHOT_KEYBIND = "Home"; // Default Bind

//keys in bakkesmod config file
const std::string SCHAT_PATHIMG = "schat_pathimg";
const std::string SCHAT_HOTKEY = "schat_hotkey";
const std::string SCHAT_WIDTH_CAPTURE = "schat_width";
const std::string SCHAT_HEIGHT_CAPTURE = "schat_height";
const std::string SCHAT_OFFSETX_CAPTURE = "schat_offsetx";
const std::string SCHAT_OFFSETY_CAPTURE = "schat_offsety";

class ScreenshotChat: public BakkesMod::Plugin::BakkesModPlugin//, public BakkesMod::Plugin::PluginSettingsWindow/*, public BakkesMod::Plugin::PluginWindow*/
{

	void GenerateSettingsFile();
	void OpenFolder();

	void TakeScreenshot();
	void WriteImage(int x, int y, int w, int h, std::string FilenameImage);
	
	std::string getTimeStr();
	void render(CanvasWrapper canvas);
	bool isEnabled();

	void setEnabled(bool enabled);

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	std::shared_ptr<bool> enabled = std::make_shared<bool>(true);


	std::string trim(const std::string& s)
	{
		auto start = s.begin();
		while (start != s.end() && std::isspace(*start)) {
			start++;
		}

		auto end = s.end();
		do {
			end--;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return std::string(start, end + 1);
	}
};

