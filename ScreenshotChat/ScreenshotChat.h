#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

#include <atlbase.h>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr const char* DEFAULT_SCREENSHOT_KEYBIND = "Home"; // Default Bind
const std::string SCHAT_PATHIMG = "schat_pathimg"; //key in bakkesmod config file
const std::string SCHAT_HOTKEY = "schat_hotkey"; //key in bakkesmod config file
const std::string SCHAT_WIDTH_CAPTURE = "schat_width"; //key in bakkesmod config file
const std::string SCHAT_HEIGHT_CAPTURE = "schat_height"; //key in bakkesmod config file
const std::string SCHAT_OFFSETX_CAPTURE = "schat_offsetx"; //key in bakkesmod config file
const std::string SCHAT_OFFSETY_CAPTURE = "schat_offsety"; //key in bakkesmod config file

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

