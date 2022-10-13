#include "pch.h"

#include <fstream>

#include "ScreenshotChat.h"
#include <shellapi.h>

// https://bakkesmod.fandom.com/wiki/Plugin_settings_files

#define blank setFile << '\n'
#define text(x) setFile << "9|" << x; blank
#define input(label, var) setFile << "12|" << label << "|" << var; blank
#define sameline setFile << "7"; blank
#define separator setFile << "8"; blank
#define slider(label, var, min, max) setFile << "5|" << label << "|" << var << "|" << min << "|" << max; blank
#define color(label, var) setFile << "13|" << label << "|" << var; blank
#define dropdown(label, var, options) setFile << "6|" << label << "|" << var << "|" << options; blank
#define button(label, fn) setFile << "0|" << label << "|" << fn; blank

#define KEYBINDS "None@None&F1@F1&F3@F3&F4@F4&F5@F5&F7@F7&F8@F8&F9@F9&F10@F10&F11@F11&F12@F12&A@A&B@B&C@C&D@D&E@E&F@F&G@G&H@H&I@I&J@J&K@K&L@L&M@M&N@N&O@O&P@P&Q@Q&R@R&S@S&T@T&U@U&V@V&W@W&X@X&Y@Y&Z@Z&Escape@Escape&Tab@Tab&Tilde@Tilde&ScrollLock@ScrollLock&Pause@Pause&one@one&two@two&three@three&four@four&five@five&six@six&seven@seven&eight@eight&nine@nine&zero@zero&Underscore@Underscore&Equals@Equals&Backslash@Backslash&LeftBracket@LeftBracket&RightBracket@RightBracket&Enter@Enter&CapsLock@CapsLock&Semicolon@Semicolon&Quote@Quote&LeftShift@LeftShift&Comma@Comma&Period@Period&Slash@Slash&RightShift@RightShift&LeftControl@LeftControl&LeftAlt@LeftAlt&SpaceBar@SpaceBar&RightAlt@RightAlt&RightControl@RightControl&Left@Left&Up@Up&Down@Down&Right@Right&Home@Home&End@End&Insert@Insert&PageUp@PageUp&Delete@Delete&PageDown@PageDown&NumLock@NumLock&Divide@Divide&Multiply@Multiply&Subtract@Subtract&Add@Add&NumPadOne@NumPadOne&NumPadTwo@NumPadTwo&NumPadThree@NumPadThree&NumPadFour@NumPadFour&NumPadFive@NumPadFive&NumPadSix@NumPadSix&NumPadSeven@NumPadSeven&NumPadEight@NumPadEight&NumPadNine@NumPadNine&NumPadZero@NumPadZero&Decimal@Decimal&LeftMouseButton@LeftMouseButton&RightMouseButton@RightMouseButton&ThumbMouseButton@ThumbMouseButton&ThumbMouseButton2@ThumbMouseButton2&MouseScrollUp@MouseScrollUp&MouseScrollDown@MouseScrollDown&MouseX@MouseX&MouseY@MouseY&XboxTypeS_LeftThumbStick@XboxTypeS_LeftThumbStick&XboxTypeS_RightThumbStick@XboxTypeS_RightThumbStick&XboxTypeS_DPad_Up@XboxTypeS_DPad_Up&XboxTypeS_DPad_Left@XboxTypeS_DPad_Left&XboxTypeS_DPad_Right@XboxTypeS_DPad_Right&XboxTypeS_DPad_Down@XboxTypeS_DPad_Down&XboxTypeS_Back@XboxTypeS_Back&XboxTypeS_Start@XboxTypeS_Start&XboxTypeS_Y@XboxTypeS_Y&XboxTypeS_X@XboxTypeS_X&XboxTypeS_B@XboxTypeS_B&XboxTypeS_A@XboxTypeS_A&XboxTypeS_LeftShoulder@XboxTypeS_LeftShoulder&XboxTypeS_RightShoulder@XboxTypeS_RightShoulder&XboxTypeS_LeftTrigger@XboxTypeS_LeftTrigger&XboxTypeS_RightTrigger@XboxTypeS_RightTrigger&XboxTypeS_LeftTriggerAxis@XboxTypeS_LeftTriggerAxis&XboxTypeS_RightTriggerAxis@XboxTypeS_RightTriggerAxis&XboxTypeS_LeftX@XboxTypeS_LeftX&XboxTypeS_LeftY@XboxTypeS_LeftY&XboxTypeS_RightX@XboxTypeS_RightX&XboxTypeS_RightY@XboxTypeS_RightY"


// Plugin Settings Window code here
void ScreenshotChat::GenerateSettingsFile()
{
	cvarManager->log("enregistrement fichier en cours");
	std::ofstream setFile(this->gameWrapper->GetBakkesModPath() / "plugins" / "settings" / "ScreenshotChat.set");

	setFile << "Screenshot Chat\n";

	blank;
	separator;
	blank;

	text("Destination folder");
	input("Folder", SCHAT_PATHIMG);
	sameline;
	button("Open in Windows Explorer", "OpenFolder");

	blank;
	separator;
	blank;

	text("Adjust Capture zone");
	slider("Width", SCHAT_WIDTH_CAPTURE, 100, 800);
	//sameline;
	slider("Height", SCHAT_HEIGHT_CAPTURE, 100, 800);

	blank;
	separator;
	blank;

	text("Offset Capture");
	slider("Offset X", SCHAT_OFFSETX_CAPTURE, -100, 100);
	//sameline;
	slider("Offset Y", SCHAT_OFFSETY_CAPTURE, -100, 100);

	blank;
	separator;
	blank;
	separator;
	text("Bindings (this creates bindings in the Bindings tab)");
	dropdown("", SCHAT_HOTKEY, KEYBINDS);

	separator;
	text(std::string("Created by Aiglenoir - ") + plugin_version);

	setFile.close();
	cvarManager->log("enregistrement fichier ok");
	cvarManager->executeCommand("cl_settings_refreshplugins");
}

void ScreenshotChat::OpenFolder() {
	cvarManager->log("Opening Explorer: " + cvarManager->getCvar(SCHAT_PATHIMG).getStringValue());
	ShellExecuteA(NULL, "explore", cvarManager->getCvar(SCHAT_PATHIMG).getStringValue().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

/*
std::string ScreenshotChat::GetPluginName() {
	return "ScreenshotChat";
}

void ScreenshotChat::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> ScreenshotChat

//void ScreenshotChat::RenderSettings() {
	CVarWrapper folderCvar = (cvarManager->getCvar("schat_pathimg"));
	CVarWrapper hotkeyCvar = (cvarManager->getCvar("schat_hotkey"));
	std::string folderStr = trim(folderCvar.getStringValue());
	std::string hotkeyStr = trim(hotkeyCvar.getStringValue());

	ImGui::TextUnformatted("Save to: ");
	//static char buf1[255] = "";
	if (ImGui::InputText("Select folder", folderStr.data(), 255)) {
		folderCvar.setValue(trim(folderStr));
	}

	ImGui::SameLine();
	if (ImGui::Button("Open folder")) {
		cvarManager->log("Open the folder now");
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Bind for screenshot: ");
	//static char buf2[64] = "";
	if (ImGui::InputText("Hotkey", hotkeyStr.data(), 64)) {
		hotkeyCvar.setValue(trim(hotkeyStr));
	}

	ImGui::Separator();

	}
*/