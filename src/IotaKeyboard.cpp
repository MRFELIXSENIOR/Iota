#include "IotaKeyboard.hpp"
#include "IotaEvent.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaApplication.hpp"

#include <vector>
#include <sol/sol.hpp>

using namespace iota;
using namespace Keyboard;

static std::map<SDL_Scancode, KeyCode> KeyCodeEntries = {
	{ SDL_SCANCODE_UNKNOWN, KeyCode::UnknownKey },
	{ SDL_SCANCODE_A, KeyCode::A },
	{ SDL_SCANCODE_B, KeyCode::B },
	{ SDL_SCANCODE_C, KeyCode::C },
	{ SDL_SCANCODE_D, KeyCode::D },
	{ SDL_SCANCODE_E, KeyCode::E },
	{ SDL_SCANCODE_F, KeyCode::F },
	{ SDL_SCANCODE_G, KeyCode::G },
	{ SDL_SCANCODE_H, KeyCode::H },
	{ SDL_SCANCODE_I, KeyCode::I },
	{ SDL_SCANCODE_J, KeyCode::J },
	{ SDL_SCANCODE_K, KeyCode::K },
	{ SDL_SCANCODE_L, KeyCode::L },
	{ SDL_SCANCODE_M, KeyCode::M },
	{ SDL_SCANCODE_N, KeyCode::N },
	{ SDL_SCANCODE_O, KeyCode::O },
	{ SDL_SCANCODE_P, KeyCode::P },
	{ SDL_SCANCODE_Q, KeyCode::Q },
	{ SDL_SCANCODE_R, KeyCode::R },
	{ SDL_SCANCODE_S, KeyCode::S },
	{ SDL_SCANCODE_T, KeyCode::T },
	{ SDL_SCANCODE_U, KeyCode::U },
	{ SDL_SCANCODE_V, KeyCode::V },
	{ SDL_SCANCODE_W, KeyCode::W },
	{ SDL_SCANCODE_X, KeyCode::X },
	{ SDL_SCANCODE_Y, KeyCode::Y },
	{ SDL_SCANCODE_Z, KeyCode::Z },
	{ SDL_SCANCODE_1, KeyCode::One },
	{ SDL_SCANCODE_2, KeyCode::Two },
	{ SDL_SCANCODE_3, KeyCode::Three },
	{ SDL_SCANCODE_4, KeyCode::Four },
	{ SDL_SCANCODE_5, KeyCode::Five },
	{ SDL_SCANCODE_6, KeyCode::Six },
	{ SDL_SCANCODE_7, KeyCode::Seven },
	{ SDL_SCANCODE_8, KeyCode::Eight },
	{ SDL_SCANCODE_9, KeyCode::Nine },
	{ SDL_SCANCODE_0, KeyCode::Zero },
	{ SDL_SCANCODE_RETURN, KeyCode::Return },
	{ SDL_SCANCODE_ESCAPE, KeyCode::Escape },
	{ SDL_SCANCODE_BACKSPACE, KeyCode::Backspace },
	{ SDL_SCANCODE_TAB, KeyCode::Tab },
	{ SDL_SCANCODE_SPACE, KeyCode::Space },
	{ SDL_SCANCODE_MINUS, KeyCode::Minux },
	{ SDL_SCANCODE_EQUALS, KeyCode::Equals },
	{ SDL_SCANCODE_LEFTBRACKET, KeyCode::LeftBracket },
	{ SDL_SCANCODE_RIGHTBRACKET, KeyCode::RightBracket },
	{ SDL_SCANCODE_BACKSLASH, KeyCode::Backslash },
	{ SDL_SCANCODE_NONUSHASH, KeyCode::Nonuslash },
	{ SDL_SCANCODE_SEMICOLON, KeyCode::SemiColon },
	{ SDL_SCANCODE_APOSTROPHE, KeyCode::Apostrophe },
	{ SDL_SCANCODE_GRAVE, KeyCode::Grave },
	{ SDL_SCANCODE_COMMA, KeyCode::Comma },
	{ SDL_SCANCODE_PERIOD, KeyCode::Period },
	{ SDL_SCANCODE_SLASH, KeyCode::Slash },
	{ SDL_SCANCODE_CAPSLOCK, KeyCode::Capslock },
	{ SDL_SCANCODE_F1, KeyCode::F1 },
	{ SDL_SCANCODE_F2, KeyCode::F2 },
	{ SDL_SCANCODE_F3, KeyCode::F3 },
	{ SDL_SCANCODE_F4, KeyCode::F4 },
	{ SDL_SCANCODE_F5, KeyCode::F5 },
	{ SDL_SCANCODE_F6, KeyCode::F6 },
	{ SDL_SCANCODE_F7, KeyCode::F7 },
	{ SDL_SCANCODE_F8, KeyCode::F8 },
	{ SDL_SCANCODE_F9, KeyCode::F9 },
	{ SDL_SCANCODE_F10, KeyCode::F10 },
	{ SDL_SCANCODE_F11, KeyCode::F11 },
	{ SDL_SCANCODE_F12, KeyCode::F12 },
	{ SDL_SCANCODE_PRINTSCREEN, KeyCode::PrintScreen },
	{ SDL_SCANCODE_SCROLLLOCK, KeyCode::ScrollLock },
	{ SDL_SCANCODE_PAUSE, KeyCode::Pause },
	{ SDL_SCANCODE_INSERT, KeyCode::Insert },
	{ SDL_SCANCODE_HOME, KeyCode::Home },
	{ SDL_SCANCODE_PAGEUP, KeyCode::PageUp },
	{ SDL_SCANCODE_DELETE, KeyCode::Delete },
	{ SDL_SCANCODE_END, KeyCode::End },
	{ SDL_SCANCODE_PAGEDOWN, KeyCode::PageDown },
	{ SDL_SCANCODE_RIGHT, KeyCode::Right },
	{ SDL_SCANCODE_LEFT, KeyCode::Left },
	{ SDL_SCANCODE_DOWN, KeyCode::Down },
	{ SDL_SCANCODE_UP, KeyCode::Up },
	{ SDL_SCANCODE_NUMLOCKCLEAR, KeyCode::Numlock },
	{ SDL_SCANCODE_KP_DIVIDE, KeyCode::NumpadDivide },
	{ SDL_SCANCODE_KP_MULTIPLY, KeyCode::NumpadMultiply },
	{ SDL_SCANCODE_KP_MINUS, KeyCode::NumpadMinus },
	{ SDL_SCANCODE_KP_PLUS, KeyCode::NumpadPlus },
	{ SDL_SCANCODE_KP_ENTER, KeyCode::NumpadEnter },
	{ SDL_SCANCODE_KP_1, KeyCode::NumpadOne },
	{ SDL_SCANCODE_KP_2, KeyCode::NumpadTwo },
	{ SDL_SCANCODE_KP_3, KeyCode::NumpadThree },
	{ SDL_SCANCODE_KP_4, KeyCode::NumpadFour },
	{ SDL_SCANCODE_KP_5, KeyCode::NumpadFive },
	{ SDL_SCANCODE_KP_6, KeyCode::NumpadSix },
	{ SDL_SCANCODE_KP_7, KeyCode::NumpadSeven },
	{ SDL_SCANCODE_KP_8, KeyCode::NumpadEight },
	{ SDL_SCANCODE_KP_9, KeyCode::NumpadNine },
	{ SDL_SCANCODE_KP_0, KeyCode::NumpadZero },
	{ SDL_SCANCODE_KP_PERIOD, KeyCode::NumpadPeriod },
	{ SDL_SCANCODE_NONUSBACKSLASH, KeyCode::NonusBackslash },
	{ SDL_SCANCODE_APPLICATION, KeyCode::Application },
	{ SDL_SCANCODE_POWER, KeyCode::Power },
	{ SDL_SCANCODE_KP_EQUALS, KeyCode::NumpadEquals },
	{ SDL_SCANCODE_F13, KeyCode::F13 },
	{ SDL_SCANCODE_F14, KeyCode::F14 },
	{ SDL_SCANCODE_F15, KeyCode::F15 },
	{ SDL_SCANCODE_F16, KeyCode::F16 },
	{ SDL_SCANCODE_F17, KeyCode::F17 },
	{ SDL_SCANCODE_F18, KeyCode::F18 },
	{ SDL_SCANCODE_F19, KeyCode::F19 },
	{ SDL_SCANCODE_F20, KeyCode::F20 },
	{ SDL_SCANCODE_F21, KeyCode::F21 },
	{ SDL_SCANCODE_F22, KeyCode::F22 },
	{ SDL_SCANCODE_F23, KeyCode::F23 },
	{ SDL_SCANCODE_F24, KeyCode::F24 },
	{ SDL_SCANCODE_EXECUTE, KeyCode::Execute },
	{ SDL_SCANCODE_HELP, KeyCode::Help },
	{ SDL_SCANCODE_MENU, KeyCode::Menu },
	{ SDL_SCANCODE_SELECT, KeyCode::Select },
	{ SDL_SCANCODE_STOP, KeyCode::Stop },
	{ SDL_SCANCODE_AGAIN, KeyCode::Again },
	{ SDL_SCANCODE_UNDO, KeyCode::Undo },
	{ SDL_SCANCODE_CUT, KeyCode::Cut },
	{ SDL_SCANCODE_COPY, KeyCode::Copy },
	{ SDL_SCANCODE_PASTE, KeyCode::Paste },
	{ SDL_SCANCODE_FIND, KeyCode::Find },
	{ SDL_SCANCODE_MUTE, KeyCode::Mute },
	{ SDL_SCANCODE_VOLUMEUP, KeyCode::VolumeUp },
	{ SDL_SCANCODE_VOLUMEDOWN, KeyCode::VolumeDown },
	{ SDL_SCANCODE_KP_COMMA, KeyCode::NumpadComma },
	{ SDL_SCANCODE_KP_EQUALSAS400, KeyCode::NumpadEqualsAs400 },
	{ SDL_SCANCODE_INTERNATIONAL1, KeyCode::International1 },
	{ SDL_SCANCODE_INTERNATIONAL2, KeyCode::International2 },
	{ SDL_SCANCODE_INTERNATIONAL3, KeyCode::International3 },
	{ SDL_SCANCODE_INTERNATIONAL4, KeyCode::International4 },
	{ SDL_SCANCODE_INTERNATIONAL5, KeyCode::International5 },
	{ SDL_SCANCODE_INTERNATIONAL6, KeyCode::International6 },
	{ SDL_SCANCODE_INTERNATIONAL7, KeyCode::International7 },
	{ SDL_SCANCODE_INTERNATIONAL8, KeyCode::International8 },
	{ SDL_SCANCODE_INTERNATIONAL9, KeyCode::International9 },
	{ SDL_SCANCODE_LANG1, KeyCode::Lang1 },
	{ SDL_SCANCODE_LANG2, KeyCode::Lang2 },
	{ SDL_SCANCODE_LANG3, KeyCode::Lang3 },
	{ SDL_SCANCODE_LANG4, KeyCode::Lang4 },
	{ SDL_SCANCODE_LANG5, KeyCode::Lang5 },
	{ SDL_SCANCODE_LANG6, KeyCode::Lang6 },
	{ SDL_SCANCODE_LANG7, KeyCode::Lang7 },
	{ SDL_SCANCODE_LANG8, KeyCode::Lang8 },
	{ SDL_SCANCODE_LANG9, KeyCode::Lang9 },
	{ SDL_SCANCODE_ALTERASE, KeyCode::AltErase },
	{ SDL_SCANCODE_SYSREQ, KeyCode::SysRq },
	{ SDL_SCANCODE_CANCEL, KeyCode::Cancel },
	{ SDL_SCANCODE_CLEAR, KeyCode::Clear },
	{ SDL_SCANCODE_PRIOR, KeyCode::Prior },
	{ SDL_SCANCODE_RETURN2, KeyCode::Return2 },
	{ SDL_SCANCODE_SEPARATOR, KeyCode::Separator },
	{ SDL_SCANCODE_OUT, KeyCode::Out },
	{ SDL_SCANCODE_OPER, KeyCode::Oper },
	{ SDL_SCANCODE_CLEARAGAIN, KeyCode::ClearAgain },
	{ SDL_SCANCODE_CRSEL, KeyCode::CrSel },
	{ SDL_SCANCODE_EXSEL, KeyCode::ExSel },
	{ SDL_SCANCODE_KP_00, KeyCode::NumpadDoubleZero },
	{ SDL_SCANCODE_KP_000, KeyCode::NumpadTripleZero },
	{ SDL_SCANCODE_THOUSANDSSEPARATOR, KeyCode::ThousandSeparator },
	{ SDL_SCANCODE_DECIMALSEPARATOR, KeyCode::DecimalSeparator },
	{ SDL_SCANCODE_CURRENCYUNIT, KeyCode::CurrencyUnit },
	{ SDL_SCANCODE_CURRENCYSUBUNIT, KeyCode::CurrencySubUnit },
	{ SDL_SCANCODE_KP_LEFTPAREN, KeyCode::NumpadLeftParenthesis },
	{ SDL_SCANCODE_KP_RIGHTPAREN, KeyCode::NumpadRightParenthesis },
	{ SDL_SCANCODE_KP_LEFTBRACE, KeyCode::NumpadLeftBrace },
	{ SDL_SCANCODE_KP_RIGHTBRACE, KeyCode::NumpadRightBrace },
	{ SDL_SCANCODE_KP_TAB, KeyCode::NumpadTab },
	{ SDL_SCANCODE_KP_BACKSPACE, KeyCode::NumpadBackspace },
	{ SDL_SCANCODE_KP_A, KeyCode::NumpadA },
	{ SDL_SCANCODE_KP_B, KeyCode::NumpadB },
	{ SDL_SCANCODE_KP_C, KeyCode::NumpadC },
	{ SDL_SCANCODE_KP_D, KeyCode::NumpadD },
	{ SDL_SCANCODE_KP_E, KeyCode::NumpadE },
	{ SDL_SCANCODE_KP_F, KeyCode::NumpadF },
	{ SDL_SCANCODE_KP_XOR, KeyCode::NumpadXOR },
	{ SDL_SCANCODE_KP_POWER, KeyCode::NumpadPower },
	{ SDL_SCANCODE_KP_PERCENT, KeyCode::NumpadPercent },
	{ SDL_SCANCODE_KP_LESS, KeyCode::NumpadLess },
	{ SDL_SCANCODE_KP_GREATER, KeyCode::NumpadGreater },
	{ SDL_SCANCODE_KP_AMPERSAND, KeyCode::NumpadAmpersand },
	{ SDL_SCANCODE_KP_DBLAMPERSAND, KeyCode::NumpadDoubleAmpersand },
	{ SDL_SCANCODE_KP_VERTICALBAR, KeyCode::NumpadVerticalBar },
	{ SDL_SCANCODE_KP_DBLVERTICALBAR, KeyCode::NumpadDoubleVerticalBar },
	{ SDL_SCANCODE_KP_COLON, KeyCode::NumpadColon },
	{ SDL_SCANCODE_KP_HASH, KeyCode::NumpadHash },
	{ SDL_SCANCODE_KP_SPACE, KeyCode::NumpadSpace },
	{ SDL_SCANCODE_KP_AT, KeyCode::NumpadAt },
	{ SDL_SCANCODE_KP_EXCLAM, KeyCode::NumpadExclamation },
	{ SDL_SCANCODE_KP_MEMSTORE, KeyCode::NumpadMemStore },
	{ SDL_SCANCODE_KP_MEMRECALL, KeyCode::NumpadMemRecall },
	{ SDL_SCANCODE_KP_MEMCLEAR, KeyCode::NumpadMemClear },
	{ SDL_SCANCODE_KP_MEMADD, KeyCode::NumpadMemAdd },
	{ SDL_SCANCODE_KP_MEMSUBTRACT, KeyCode::NumpadMemSubtract },
	{ SDL_SCANCODE_KP_MEMMULTIPLY, KeyCode::NumpadMemMultiply },
	{ SDL_SCANCODE_KP_MEMDIVIDE, KeyCode::NumpadMemDivide },
	{ SDL_SCANCODE_KP_PLUSMINUS, KeyCode::NumpadPlusMinus },
	{ SDL_SCANCODE_KP_CLEAR, KeyCode::NumpadClear },
	{ SDL_SCANCODE_KP_CLEARENTRY, KeyCode::NumpadClearEntry },
	{ SDL_SCANCODE_KP_BINARY, KeyCode::NumpadBinary },
	{ SDL_SCANCODE_KP_OCTAL, KeyCode::NumpadOctal },
	{ SDL_SCANCODE_KP_DECIMAL, KeyCode::NumpadDecimal },
	{ SDL_SCANCODE_KP_HEXADECIMAL, KeyCode::NumpadHexadecimal },
	{ SDL_SCANCODE_LCTRL, KeyCode::LeftControl },
	{ SDL_SCANCODE_LSHIFT, KeyCode::LeftShift },
	{ SDL_SCANCODE_LALT, KeyCode::LeftAlt },
	{ SDL_SCANCODE_LGUI, KeyCode::LeftWindows },
	{ SDL_SCANCODE_RCTRL, KeyCode::RightControl },
	{ SDL_SCANCODE_RSHIFT, KeyCode::RightShift },
	{ SDL_SCANCODE_RALT, KeyCode::RightAlt },
	{ SDL_SCANCODE_RGUI, KeyCode::RightWindows },
	{ SDL_SCANCODE_MODE, KeyCode::Mode },
	{ SDL_SCANCODE_AUDIONEXT, KeyCode::AudioNext },
	{ SDL_SCANCODE_AUDIOPREV, KeyCode::AudioPrevious },
	{ SDL_SCANCODE_AUDIOSTOP, KeyCode::AudioStop },
	{ SDL_SCANCODE_AUDIOPLAY, KeyCode::AudioPlay },
	{ SDL_SCANCODE_AUDIOMUTE, KeyCode::AudioMute },
	{ SDL_SCANCODE_MEDIASELECT, KeyCode::MediaSelect },
	{ SDL_SCANCODE_WWW, KeyCode::WWW },
	{ SDL_SCANCODE_MAIL, KeyCode::Mail },
	{ SDL_SCANCODE_CALCULATOR, KeyCode::Calculator },
	{ SDL_SCANCODE_COMPUTER, KeyCode::Computer },
	{ SDL_SCANCODE_AC_SEARCH, KeyCode::ApplicationControlSearch },
	{ SDL_SCANCODE_AC_HOME, KeyCode::ApplicationControlHome },
	{ SDL_SCANCODE_AC_BACK, KeyCode::ApplicationControlBack },
	{ SDL_SCANCODE_AC_FORWARD, KeyCode::ApplicationControlForward },
	{ SDL_SCANCODE_AC_STOP, KeyCode::ApplicationControlStop },
	{ SDL_SCANCODE_AC_REFRESH, KeyCode::ApplicationControlRefresh },
	{ SDL_SCANCODE_AC_BOOKMARKS, KeyCode::ApplicationControlBookmarks },
	{ SDL_SCANCODE_BRIGHTNESSDOWN, KeyCode::BrightnessDown },
	{ SDL_SCANCODE_BRIGHTNESSUP, KeyCode::BrightnessUp },
	{ SDL_SCANCODE_DISPLAYSWITCH, KeyCode::DisplaySwitch },
	{ SDL_SCANCODE_KBDILLUMTOGGLE, KeyCode::KBDIllumToggle },
	{ SDL_SCANCODE_KBDILLUMDOWN, KeyCode::KBDIllumDown },
	{ SDL_SCANCODE_KBDILLUMUP, KeyCode::KBDIllumUp },
	{ SDL_SCANCODE_EJECT, KeyCode::Eject },
	{ SDL_SCANCODE_SLEEP, KeyCode::Sleep },
	{ SDL_SCANCODE_APP1, KeyCode::App1 },
	{ SDL_SCANCODE_APP2, KeyCode::App2 },
	{ SDL_SCANCODE_AUDIOREWIND, KeyCode::AudioRewind },
	{ SDL_SCANCODE_AUDIOFASTFORWARD, KeyCode::AudioFastForward }
};
static std::map<KeyCode, std::string> KeystringEntries = {
	{ KeyCode::UnknownKey, "UnknownKey" },
	{ KeyCode::A, "A" },
	{ KeyCode::B, "B" },
	{ KeyCode::C, "C" },
	{ KeyCode::D, "D" },
	{ KeyCode::E, "E" },
	{ KeyCode::F, "F" },
	{ KeyCode::G, "G" },
	{ KeyCode::H, "H" },
	{ KeyCode::I, "I" },
	{ KeyCode::J, "J" },
	{ KeyCode::K, "K" },
	{ KeyCode::L, "L" },
	{ KeyCode::M, "M" },
	{ KeyCode::N, "N" },
	{ KeyCode::O, "O" },
	{ KeyCode::P, "P" },
	{ KeyCode::Q, "Q" },
	{ KeyCode::R, "R" },
	{ KeyCode::S, "S" },
	{ KeyCode::T, "T" },
	{ KeyCode::U, "U" },
	{ KeyCode::V, "V" },
	{ KeyCode::W, "W" },
	{ KeyCode::X, "X" },
	{ KeyCode::Y, "Y" },
	{ KeyCode::Z, "Z" },
	{ KeyCode::One, "One" },
	{ KeyCode::Two, "Two" },
	{ KeyCode::Three, "Three" },
	{ KeyCode::Four, "Four" },
	{ KeyCode::Five, "Five" },
	{ KeyCode::Six, "Six" },
	{ KeyCode::Seven, "Seven" },
	{ KeyCode::Eight, "Eight" },
	{ KeyCode::Nine, "Nine" },
	{ KeyCode::Zero, "Ten" },
	{ KeyCode::Return, "Return" },
	{ KeyCode::Escape, "Escape" },
	{ KeyCode::Backspace, "Backspace" },
	{ KeyCode::Tab, "Tab" },
	{ KeyCode::Space, "Space" },
	{ KeyCode::Minux, "Minus" },
	{ KeyCode::Equals, "Equals" },
	{ KeyCode::LeftBracket, "LeftBracket" },
	{ KeyCode::RightBracket, "RightBracket" },
	{ KeyCode::Backslash, "Backslash" },
	{ KeyCode::Nonuslash, "Nonuslash" },
	{ KeyCode::SemiColon, "SemiColon" },
	{ KeyCode::Apostrophe, "Apostrophe" },
	{ KeyCode::Grave, "Grave" },
	{ KeyCode::Comma, "Comma" },
	{ KeyCode::Period, "Period" },
	{ KeyCode::Slash, "Slash" },
	{ KeyCode::Capslock, "Capslock" },
	{ KeyCode::F1, "F1" },
	{ KeyCode::F2, "F2" },
	{ KeyCode::F3, "F3" },
	{ KeyCode::F4, "F4" },
	{ KeyCode::F5, "F5" },
	{ KeyCode::F6, "F6" },
	{ KeyCode::F7, "F7" },
	{ KeyCode::F8, "F8" },
	{ KeyCode::F9, "F9" },
	{ KeyCode::F10, "F10" },
	{ KeyCode::F11, "F11" },
	{ KeyCode::F12, "F12" },
	{ KeyCode::PrintScreen, "PrintScreen" },
	{ KeyCode::ScrollLock, "ScrollLock" },
	{ KeyCode::Pause, "Pause" },
	{ KeyCode::Insert, "Insert" },
	{ KeyCode::Home, "Home" },
	{ KeyCode::PageUp, "PageUp" },
	{ KeyCode::Delete, "Delete" },
	{ KeyCode::End, "End" },
	{ KeyCode::PageDown, "PageDown" },
	{ KeyCode::Right, "Right" },
	{ KeyCode::Left, "Left" },
	{ KeyCode::Down, "Down" },
	{ KeyCode::Up, "Up" },
	{ KeyCode::Numlock, "Numlock" },
	{ KeyCode::NumpadDivide, "NumpadDivide" },
	{ KeyCode::NumpadMultiply, "NumpadMultiply" },
	{ KeyCode::NumpadMinus, "NumpadMinus" },
	{ KeyCode::NumpadPlus, "NumpadPlus" },
	{ KeyCode::NumpadEnter, "NumpadEnter" },
	{ KeyCode::NumpadOne, "NumpadOne" },
	{ KeyCode::NumpadTwo, "NumpadTwo" },
	{ KeyCode::NumpadThree, "NumpadThree" },
	{ KeyCode::NumpadFour, "NumpadFour" },
	{ KeyCode::NumpadFive, "NumpadFive" },
	{ KeyCode::NumpadSix, "NumpadSix" },
	{ KeyCode::NumpadSeven, "NumpadSeven" },
	{ KeyCode::NumpadEight, "NumpadEight" },
	{ KeyCode::NumpadEight, "NumpadEight" },
	{ KeyCode::NumpadNine, "NumpadNine" },
	{ KeyCode::NumpadZero, "NumpadZero" },
	{ KeyCode::NumpadPeriod, "NumpadPeriod" },
	{ KeyCode::NonusBackslash, "NonusBackslash" },
	{ KeyCode::Application, "Application" },
	{ KeyCode::Power, "Power" },
	{ KeyCode::NumpadEquals, "NumpadEquals" },
	{ KeyCode::F13, "F13" },
	{ KeyCode::F14, "F14" },
	{ KeyCode::F15, "F15" },
	{ KeyCode::F16, "F16" },
	{ KeyCode::F17, "F17" },
	{ KeyCode::F18, "F18" },
	{ KeyCode::F19, "F19" },
	{ KeyCode::F20, "F20" },
	{ KeyCode::F21, "F21" },
	{ KeyCode::F22, "F22" },
	{ KeyCode::F23, "F23" },
	{ KeyCode::F24, "F24" },
	{ KeyCode::Execute, "Execute" },
	{ KeyCode::Help, "Help" },
	{ KeyCode::Menu, "Menu" },
	{ KeyCode::Select, "Select" },
	{ KeyCode::Stop, "Stop" },
	{ KeyCode::Again, "Again" },
	{ KeyCode::Undo, "Undo" },
	{ KeyCode::Cut, "Cut" },
	{ KeyCode::Copy, "Copy" },
	{ KeyCode::Paste, "Paste" },
	{ KeyCode::Find, "Find" },
	{ KeyCode::Mute, "Mute" },
	{ KeyCode::VolumeUp, "VolumeUp" },
	{ KeyCode::VolumeDown, "VolumeDown" },
	{ KeyCode::NumpadComma, "NumpadComma" },
	{ KeyCode::NumpadEqualsAs400, "NumpadEqualsAs400" },
	{ KeyCode::International1, "International1" },
	{ KeyCode::International2, "International2" },
	{ KeyCode::International3, "International3" },
	{ KeyCode::International4, "International4" },
	{ KeyCode::International5, "International5" },
	{ KeyCode::International6, "International6" },
	{ KeyCode::International7, "International7" },
	{ KeyCode::International8, "International8" },
	{ KeyCode::International9, "International9" },
	{ KeyCode::Lang1, "Lang1" },
	{ KeyCode::Lang2, "Lang2" },
	{ KeyCode::Lang3, "Lang3" },
	{ KeyCode::Lang4, "Lang4" },
	{ KeyCode::Lang5, "Lang5" },
	{ KeyCode::Lang6, "Lang6" },
	{ KeyCode::Lang7, "Lang7" },
	{ KeyCode::Lang8, "Lang8" },
	{ KeyCode::Lang9, "Lang9" },
	{ KeyCode::AltErase, "AltErase" },
	{ KeyCode::SysRq, "SysRq" },
	{ KeyCode::Cancel, "Cancel" },
	{ KeyCode::Clear, "Clear" },
	{ KeyCode::Prior, "Prior" },
	{ KeyCode::Return2, "Return2" },
	{ KeyCode::Separator, "Separator" },
	{ KeyCode::Out, "Out" },
	{ KeyCode::Oper, "Oper" },
	{ KeyCode::ClearAgain, "ClearAgain" },
	{ KeyCode::CrSel, "CrSel" },
	{ KeyCode::ExSel, "ExSel" },
	{ KeyCode::NumpadDoubleZero, "NumpadDoubleZero" },
	{ KeyCode::NumpadTripleZero, "NumpadTripleZero" },
	{ KeyCode::ThousandSeparator, "ThousandSeparator" },
	{ KeyCode::DecimalSeparator, "DecimalSeparator" },
	{ KeyCode::CurrencyUnit, "CurrencyUnit" },
	{ KeyCode::CurrencySubUnit, "CurrencySubUnit" },
	{ KeyCode::NumpadLeftParenthesis, "NumpadLeftParenthesis" },
	{ KeyCode::NumpadRightParenthesis, "NumpadRightParenthesis" },
	{ KeyCode::NumpadLeftBrace, "NumpadLeftBrace" },
	{ KeyCode::NumpadRightBrace, "NumpadRightBrace" },
	{ KeyCode::NumpadTab, "NumpadTab" },
	{ KeyCode::NumpadBackspace, "NumpadBackspace" },
	{ KeyCode::NumpadA, "NumpadA" },
	{ KeyCode::NumpadB, "NumpadB" },
	{ KeyCode::NumpadC, "NumpadC" },
	{ KeyCode::NumpadD, "NumpadD" },
	{ KeyCode::NumpadE, "NumpadE" },
	{ KeyCode::NumpadF, "NumpadF" },
	{ KeyCode::NumpadXOR, "NumpadXOR" },
	{ KeyCode::NumpadPower, "NumpadPower" },
	{ KeyCode::NumpadPercent, "NumpadPercent" },
	{ KeyCode::NumpadLess, "NumpadLess" },
	{ KeyCode::NumpadGreater, "NumpadGreater" },
	{ KeyCode::NumpadAmpersand, "NumpadAmpersand" },
	{ KeyCode::NumpadDoubleAmpersand, "NumpadDoubleAmpersand" },
	{ KeyCode::NumpadVerticalBar, "NumpadVerticalBar" },
	{ KeyCode::NumpadDoubleVerticalBar, "NumpadDoubleVerticalBar" },
	{ KeyCode::NumpadColon, "NumpadColon" },
	{ KeyCode::NumpadHash, "NumpadHash" },
	{ KeyCode::NumpadSpace, "NumpadSpace" },
	{ KeyCode::NumpadAt, "NumpadAt" },
	{ KeyCode::NumpadExclamation, "NumpadExclamation" },
	{ KeyCode::NumpadMemStore, "NumpadMemStore" },
	{ KeyCode::NumpadMemRecall, "NumpadMemRecall" },
	{ KeyCode::NumpadMemClear, "NumpadMemClear" },
	{ KeyCode::NumpadMemAdd, "NumpadMemAdd" },
	{ KeyCode::NumpadMemSubtract, "NumpadMemSubtract" },
	{ KeyCode::NumpadMemMultiply, "NumpadMemMultiply" },
	{ KeyCode::NumpadMemDivide, "NumpadMemDivide" },
	{ KeyCode::NumpadPlusMinus, "NumpadPlusMinus" },
	{ KeyCode::NumpadClear, "NumpadClear" },
	{ KeyCode::NumpadClearEntry, "NumpadClearEntry" },
	{ KeyCode::NumpadBinary, "NumpadBinary" },
	{ KeyCode::NumpadOctal, "NumpadOctal" },
	{ KeyCode::NumpadDecimal, "NumpadDecimal" },
	{ KeyCode::NumpadHexadecimal, "NumpadHexadecimal" },
	{ KeyCode::LeftControl, "LeftControl" },
	{ KeyCode::LeftShift, "LeftShift" },
	{ KeyCode::LeftAlt, "LeftAlt" },
	{ KeyCode::LeftWindows, "LeftWindows" },
	{ KeyCode::RightControl, "RightControl" },
	{ KeyCode::RightShift, "RightShift" },
	{ KeyCode::RightAlt, "RightAlt" },
	{ KeyCode::RightWindows, "RightWindows" },
	{ KeyCode::Mode, "Mode" },
	{ KeyCode::AudioNext, "AudioNext" },
	{ KeyCode::AudioPrevious, "AudioPrevious" },
	{ KeyCode::AudioStop, "AudioStop" },
	{ KeyCode::AudioPlay, "AudioPlay" },
	{ KeyCode::AudioMute, "AudioMute" },
	{ KeyCode::MediaSelect, "MediaSelect" },
	{ KeyCode::WWW, "WWW" },
	{ KeyCode::Mail, "Mail" },
	{ KeyCode::Calculator, "Calculator" },
	{ KeyCode::Computer, "Computer" },
	{ KeyCode::ApplicationControlSearch, "ApplicationControlSearch" },
	{ KeyCode::ApplicationControlHome, "ApplicationControlHome" },
	{ KeyCode::ApplicationControlBack, "ApplicationControlBack" },
	{ KeyCode::ApplicationControlForward, "ApplicationControlForward" },
	{ KeyCode::ApplicationControlStop, "ApplicationControlStop" },
	{ KeyCode::ApplicationControlRefresh, "ApplicationControlRefresh" },
	{ KeyCode::ApplicationControlBookmarks, "ApplicationControlBookmarks" },
	{ KeyCode::BrightnessDown, "BrightnessDown" },
	{ KeyCode::BrightnessUp, "BrightnessUp" },
	{ KeyCode::DisplaySwitch, "DisplaySwitch" },
	{ KeyCode::KBDIllumToggle, "KBDIllumToggle" },
	{ KeyCode::KBDIllumDown, "KBDIllumDown" },
	{ KeyCode::KBDIllumUp, "KBDIllumUp" },
	{ KeyCode::Eject, "Eject" },
	{ KeyCode::Sleep, "Sleep" },
	{ KeyCode::App1, "App1" },
	{ KeyCode::App2, "App2" },
	{ KeyCode::AudioRewind, "AudioRewind" },
	{ KeyCode::AudioFastForward, "AudioFastForward" }
};

static Event::EventSignal<KeyCode> keydown_event;
static Event::EventSignal<KeyCode> keyup_event;

static std::vector<KeyCode> keys_up;
static std::vector<KeyCode> keys_down;

SDL_Scancode Keyboard::GetKey(KeyCode key) {
	for (auto& k : KeyCodeEntries) {
		if (k.second == key) {
			return k.first;
		}
	}

	return SDL_SCANCODE_UNKNOWN;
}

KeyCode Keyboard::GetKey(SDL_Scancode scancode) {
	return KeyCodeEntries.at(scancode);
}

std::map<SDL_Scancode, KeyCode>& Keyboard::GetKeyEntries() { return KeyCodeEntries; }
std::map<KeyCode, std::string>& Keyboard::GetKeystringEntries() { return KeystringEntries; }

void Keyboard::LoadLuaSTD() {
	if (Application::IsInitialized()) return;
	sol::table& Iota = Lua::GetIota();
	sol::state& lua = Lua::GetEngineLuaState();

	Iota["Input"]["IsKeyDown"] = &IsKeyDown;
	Iota["Input"]["IsKeyRelease"] = &IsKeyReleased;

	Iota["Input"]["OnKeyDown"]["Connect"] = [](sol::function fn) {
		keydown_event.Connect([fn](KeyCode key) mutable {
			fn.call(key);
		});
	};

	Iota["Input"]["OnKeyRelease"]["Connect"] = [](sol::function fn) {
		keyup_event.Connect([fn](KeyCode key) mutable {
			fn.call(key);
		});
	};

	for (auto& k : Keyboard::GetKeyEntries()) {
		Iota["Enum"]["KeyCode"][GetKeystringEntries().at(k.second)] = (int)k.first;
	}

	Iota["Util"]["ConvertKeyCode"] = [&](KeyCode key) {
		return GetKeystringEntries().at(key);
	};
}

bool Keyboard::IsKeyDown(KeyCode key) {
	auto it = std::find(keys_down.begin(), keys_down.end(), key);
	if (it != keys_down.end()) {
		return true;
	}
	return false;
}

bool Keyboard::IsKeyReleased(KeyCode key) {
	auto it = std::find(keys_up.begin(), keys_up.end(), key);
	if (it != keys_down.end()) {
		return true;
	}
	return false;
}

void Keyboard::HandleKeyEvent(SDL_Scancode scancode, KeyState state) {
	KeyCode key = GetKey(scancode);

	if (state == KeyState::DOWN) {
		keydown_event.Fire(key);

		auto it = std::find(keys_up.begin(), keys_up.end(), key);
		if (it != keys_up.end()) {
			keys_up.erase(it);
			keys_down.push_back(key);
		}
	}
	else if (state == KeyState::RELEASE) {
		keyup_event.Fire(key);

		auto it = std::find(keys_up.begin(), keys_up.end(), key);
		if (it != keys_up.end()) {
			keys_down.erase(it);
			keys_up.push_back(key);
		}
	}
}