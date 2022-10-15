
#include "pch.h"

#include "ScreenshotChat.h"

#include <atlimage.h> // CImage gdiplus

BAKKESMOD_PLUGIN(ScreenshotChat, "Chat Screenshot", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void ScreenshotChat::TakeScreenshot()
{
	if (!gameWrapper->IsInGame() && !gameWrapper->IsInOnlineGame() && !gameWrapper->IsInFreeplay())
	{
		cvarManager->log("Screenshot Failed, not in a game!!");
		return;
	}

	//Formatting output file: path\prefix_%Y-%m-%d_%H-%M-%S.jpg
	std::string pathSave = (cvarManager->getCvar(SCHAT_PATHIMG)).getStringValue() + "\\";
	std::string prefix = "chatScreen_";
	std::string dateTime = ScreenshotChat::getTimeStr();
	std::string extension = ".jpg";

	std::string FilenameImage = pathSave + prefix + dateTime + extension;

	cvarManager->log(FilenameImage);
	int widthCapture = (cvarManager->getCvar(SCHAT_WIDTH_CAPTURE)).getIntValue();
	int heightCapture = (cvarManager->getCvar(SCHAT_HEIGHT_CAPTURE)).getIntValue();
	int offsetX = (cvarManager->getCvar(SCHAT_OFFSETX_CAPTURE)).getIntValue();
	int offsetY = (cvarManager->getCvar(SCHAT_OFFSETY_CAPTURE)).getIntValue();
	ScreenshotChat::WriteImage(offsetX, offsetY, widthCapture, heightCapture, FilenameImage);

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		render(canvas);
		});

	gameWrapper->SetTimeout([self = this](GameWrapper* gw) {
		gw->UnregisterDrawables();
		}, 1); // display canvas 1 sec

}

void ScreenshotChat::WriteImage(int x, int y, int w, int h, std::string FilenameImage) {

	int x1, y1;

	// get screen dimensions
	x1 = GetSystemMetrics(SM_XVIRTUALSCREEN); //topleft
	y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);

	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, x1, y1, w, h, hScreen, x1+x, y1+y, SRCCOPY);

	//converting bitmap to jpeg 
	std::vector<BYTE> buf;
	IStream* stream = NULL;
	HRESULT hCreateStream = CreateStreamOnHGlobal(0, TRUE, &stream);
	CImage image;
	ULARGE_INTEGER liSize;
	image.Attach(hBitmap);
	image.Save(stream, Gdiplus::ImageFormatJPEG);
	IStream_Size(stream, &liSize);
	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[0], len);
	stream->Release();

	// writing file
	FILE* fp = fopen(FilenameImage.c_str(), "wb");
	if (fp)
	{
		fwrite(buf.data(), buf.size(), 1, fp);
		fclose(fp);
	}
	
}

std::string ScreenshotChat::getTimeStr() {
	char s[30];
	struct tm tim;
	time_t now;
	now = time(NULL);
	tim = *(localtime(&now));
	std::strftime(s, 30, "%Y-%m-%d_%H-%M-%S", &tim);
	return s;
}

void ScreenshotChat::render(CanvasWrapper canvas) {

	int widthCapture = (cvarManager->getCvar(SCHAT_WIDTH_CAPTURE)).getIntValue();
	int heightCapture = (cvarManager->getCvar(SCHAT_HEIGHT_CAPTURE)).getIntValue();
	int offsetX = (cvarManager->getCvar(SCHAT_OFFSETX_CAPTURE)).getIntValue();
	int offsetY = (cvarManager->getCvar(SCHAT_OFFSETY_CAPTURE)).getIntValue();
	LinearColor colors{};
	colors.R = 0; //160
	colors.G = 0;
	colors.B = 0;
	colors.A = 255; // 100
	canvas.SetPosition(Vector2{ offsetX , offsetY });
	canvas.SetColor(colors);
	//canvas.FillBox(Vector2{ 20, 50 });
	canvas.DrawString("Capture Saved!", 1.2f, 1.2f);

}

void ScreenshotChat::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->log("Screenshot Chat Loaded!");
	//cvarManager->executeCommand("exec config.cfg"); // useless?

	Vector2 GetScreenSize = gameWrapper->GetScreenSize();
	int screenSizeX = GetScreenSize.X;
	int screenSizeY = GetScreenSize.Y;
	float displayScale = gameWrapper->GetDisplayScale();
	cvarManager->log("displayScale  = " + std::to_string(displayScale));
	float ratioScreenCapture = 4.2f; // depends of interface scale but ok for 1920*1080 100% scale

	if (screenSizeX >= 3440) {
		ratioScreenCapture = 5.8f;
	}
	else if (screenSizeX >= 2560) {
		ratioScreenCapture = 5.2f; // no screen in 27/31" to verify this default value
	}
	else if (screenSizeX >= 1920) {
		ratioScreenCapture = 4.2f;
	}
	else if (screenSizeX >= 1280) {
		ratioScreenCapture = 3.2f;
	}
	else {
		ratioScreenCapture = 2.8f;
	}

	// default settings
	std::string DEFAULT_PATH_IMG = gameWrapper->GetDataFolder().string();
	cvarManager->registerCvar(SCHAT_PATHIMG, DEFAULT_PATH_IMG, "Save into this folder", true, false, false, false, false, true);

	std::string DEFAULT_WIDTH_CAPTURE = std::to_string(static_cast<int>(float(screenSizeX) / ratioScreenCapture));
	std::string DEFAULT_HEIGHT_CAPTURE = std::to_string(static_cast<int>(float(screenSizeY) / ratioScreenCapture));
	cvarManager->registerCvar(SCHAT_WIDTH_CAPTURE, DEFAULT_WIDTH_CAPTURE, "Capture Width", true, true, 100, true, 800, true);
	cvarManager->registerCvar(SCHAT_HEIGHT_CAPTURE, DEFAULT_HEIGHT_CAPTURE, "Capture height", true, true, 100, true, 800, true);
	cvarManager->registerCvar(SCHAT_OFFSETX_CAPTURE, "22", "Offset X", true, true, -100, true, 100, true);
	cvarManager->registerCvar(SCHAT_OFFSETY_CAPTURE, "22", "Offset Y", true, true, -100, true, 100, true);
	//cvarManager->log("width " + DEFAULT_WIDTH_CAPTURE + "/ height: " + DEFAULT_HEIGHT_CAPTURE);

	cvarManager->registerCvar(SCHAT_HOTKEY, DEFAULT_SCREENSHOT_KEYBIND, "Hotkey for screenshot", true, false, false, false, false, true)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		cvarManager->setBind((cvarManager->getCvar(SCHAT_HOTKEY)).getStringValue(), "TakeScreenshot"); // rebind after hotkey changed
		cvarManager->log("New Bind = > " + (cvarManager->getCvar(SCHAT_HOTKEY)).getStringValue());
		});
	//cvarManager->log("loaded cvars: " + cvarManager->getCvar(SCHAT_HOTKEY).getStringValue() + " / " + cvarManager->getCvar(SCHAT_PATHIMG).getStringValue());

	// Bind Hotkey
	cvarManager->setBind((cvarManager->getCvar(SCHAT_HOTKEY)).getStringValue(), "TakeScreenshot");

	// function called after hotkey pressed
	cvarManager->registerNotifier(
		"TakeScreenshot",
		[this](std::vector<std::string> params) { TakeScreenshot(); },
		"Take the chat screenshot",
		PERMISSION_ALL //PERMISSION_ONLINE
	);

	GenerateSettingsFile();

	// open folder button
	cvarManager->registerNotifier(
		"OpenFolder",
		[this](std::vector<std::string> params) { OpenFolder(); },
		"Open Windows Explorer",
		PERMISSION_ALL //PERMISSION_ONLINE
	);

}

void ScreenshotChat::onUnload()
{
	cvarManager->executeCommand("writeconfig");
}


bool ScreenshotChat::isEnabled() {
	return *enabled;
}

void ScreenshotChat::setEnabled(bool enabled) {
	this->enabled = std::make_shared<bool>(enabled);
}
