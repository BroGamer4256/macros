#include "diva.h"
#include <cwchar>
#include <optional>

using namespace diva;

#pragma pack(8)
struct MapInsertionPoint {
	mapElement<i32, list<Key>> *insertionPoint;
	bool unk;
};

struct Override {
	std::vector<ButtonType> buttons;
	std::vector<Key> keys;
	i32 id;
};
std::vector<Override> Overrides;

FUNCTION_PTR (void, InsertMapElement, 0x140157A50, map<i32, list<Key>> *map, MapInsertionPoint *location, mapElement<i32, list<Key>> *newNode);

bool inited = false;
HOOK (void *, RegisterButtonPresses, 0x1401DBD70, void *a1, void *inputState, i32 buttonId) {
	if (!inited) {
		originalRegisterButtonPresses (a1, inputState, buttonId);
		for (auto override : Overrides)
			for (auto button : override.buttons)
				buttonIds[(i32)button].push_back (override.id);
		inited = true;
	}
	return originalRegisterButtonPresses (a1, inputState, buttonId);
}

HOOK (void *, RegisterButtonKeys, 0x1401DB160, void *a1) {
	void *ret = originalRegisterButtonKeys (a1);

	for (auto override : Overrides) {
		list<Key> keys;
		for (auto key : override.keys)
			keys.push_back (key);
		auto elem    = new mapElement<i32, list<Key>>;
		elem->left   = (*buttonKeys)->begin ();
		elem->parent = (*buttonKeys)->begin ();
		elem->right  = (*buttonKeys)->begin ();
		elem->key    = override.id;
		elem->value  = keys;
		elem->isNull = false;
		MapInsertionPoint a2{(*buttonKeys)->bottom (), false};
		InsertMapElement (*buttonKeys, &a2, elem);
	}

	return ret;
}

std::optional<ButtonType>
buttonStringParse (char *button) {
	std::optional<ButtonType> o = std::nullopt;
	if (strcmp (button, "CIRCLE") == 0) o = std::optional (ButtonType::CIRCLE);
	else if (strcmp (button, "CROSS") == 0) o = std::optional (ButtonType::CROSS);
	else if (strcmp (button, "TRIANGLE") == 0) o = std::optional (ButtonType::TRIANGLE);
	else if (strcmp (button, "SQUARE") == 0) o = std::optional (ButtonType::SQUARE);
	else if (strcmp (button, "SLIDE_LEFT") == 0) o = std::optional (ButtonType::SLIDE_LEFT);
	else if (strcmp (button, "SLIDE_RIGHT") == 0) o = std::optional (ButtonType::SLIDE_RIGHT);

	free (button);
	return o;
}

std::optional<Key>
keyStringParse (char *key) {
	std::optional<Key> o = std::nullopt;
	if (strcmp (key, "CONTROLLER_DPAD_UP") == 0) o = std::optional (Key::CONTROLLER_DPAD_UP);
	else if (strcmp (key, "CONTROLLER_DPAD_DOWN") == 0) o = std::optional (Key::CONTROLLER_DPAD_DOWN);
	else if (strcmp (key, "CONTROLLER_DPAD_LEFT") == 0) o = std::optional (Key::CONTROLLER_DPAD_LEFT);
	else if (strcmp (key, "CONTROLLER_DPAD_RIGHT") == 0) o = std::optional (Key::CONTROLLER_DPAD_RIGHT);
	else if (strcmp (key, "CONTROLLER_X") == 0) o = std::optional (Key::CONTROLLER_X);
	else if (strcmp (key, "CONTROLLER_Y") == 0) o = std::optional (Key::CONTROLLER_Y);
	else if (strcmp (key, "CONTROLLER_B") == 0) o = std::optional (Key::CONTROLLER_B);
	else if (strcmp (key, "CONTROLLER_A") == 0) o = std::optional (Key::CONTROLLER_A);
	else if (strcmp (key, "CONTROLLER_L1") == 0) o = std::optional (Key::CONTROLLER_L1);
	else if (strcmp (key, "CONTROLLER_R1") == 0) o = std::optional (Key::CONTROLLER_R1);
	else if (strcmp (key, "CONTROLLER_L2") == 0) o = std::optional (Key::CONTROLLER_L2);
	else if (strcmp (key, "CONTROLLER_R2") == 0) o = std::optional (Key::CONTROLLER_R2);
	else if (strcmp (key, "CONTROLLER_L3") == 0) o = std::optional (Key::CONTROLLER_L3);
	else if (strcmp (key, "CONTROLLER_R3") == 0) o = std::optional (Key::CONTROLLER_R3);
	else if (strcmp (key, "CONTROLLER_TOUCHPAD") == 0) o = std::optional (Key::CONTROLLER_TOUCHPAD);
	else if (strcmp (key, "CONTROLLER_LEFT_UP") == 0) o = std::optional (Key::CONTROLLER_LEFT_UP);
	else if (strcmp (key, "CONTROLLER_LEFT_DOWN") == 0) o = std::optional (Key::CONTROLLER_LEFT_DOWN);
	else if (strcmp (key, "CONTROLLER_LEFT_LEFT") == 0) o = std::optional (Key::CONTROLLER_LEFT_LEFT);
	else if (strcmp (key, "CONTROLLER_LEFT_RIGHT") == 0) o = std::optional (Key::CONTROLLER_LEFT_RIGHT);
	else if (strcmp (key, "CONTROLLER_RIGHT_UP") == 0) o = std::optional (Key::CONTROLLER_RIGHT_UP);
	else if (strcmp (key, "CONTROLLER_RIGHT_DOWN") == 0) o = std::optional (Key::CONTROLLER_RIGHT_DOWN);
	else if (strcmp (key, "CONTROLLER_RIGHT_LEFT") == 0) o = std::optional (Key::CONTROLLER_RIGHT_LEFT);
	else if (strcmp (key, "CONTROLLER_RIGHT_RIGHT") == 0) o = std::optional (Key::CONTROLLER_RIGHT_RIGHT);
	else if (strcmp (key, "KEYBOARD_1") == 0) o = std::optional (Key::KEYBOARD_1);
	else if (strcmp (key, "KEYBOARD_2") == 0) o = std::optional (Key::KEYBOARD_2);
	else if (strcmp (key, "KEYBOARD_3") == 0) o = std::optional (Key::KEYBOARD_3);
	else if (strcmp (key, "KEYBOARD_4") == 0) o = std::optional (Key::KEYBOARD_4);
	else if (strcmp (key, "KEYBOARD_5") == 0) o = std::optional (Key::KEYBOARD_5);
	else if (strcmp (key, "KEYBOARD_6") == 0) o = std::optional (Key::KEYBOARD_6);
	else if (strcmp (key, "KEYBOARD_7") == 0) o = std::optional (Key::KEYBOARD_7);
	else if (strcmp (key, "KEYBOARD_8") == 0) o = std::optional (Key::KEYBOARD_8);
	else if (strcmp (key, "KEYBOARD_9") == 0) o = std::optional (Key::KEYBOARD_9);
	else if (strcmp (key, "KEYBOARD_0") == 0) o = std::optional (Key::KEYBOARD_0);
	else if (strcmp (key, "KEYBOARD_MINUS") == 0) o = std::optional (Key::KEYBOARD_MINUS);
	else if (strcmp (key, "KEYBOARD_BACKSPACE") == 0) o = std::optional (Key::KEYBOARD_BACKSPACE);
	else if (strcmp (key, "KEYBOARD_TAB") == 0) o = std::optional (Key::KEYBOARD_TAB);
	else if (strcmp (key, "KEYBOARD_Q") == 0) o = std::optional (Key::KEYBOARD_Q);
	else if (strcmp (key, "KEYBOARD_W") == 0) o = std::optional (Key::KEYBOARD_W);
	else if (strcmp (key, "KEYBOARD_E") == 0) o = std::optional (Key::KEYBOARD_E);
	else if (strcmp (key, "KEYBOARD_R") == 0) o = std::optional (Key::KEYBOARD_R);
	else if (strcmp (key, "KEYBOARD_T") == 0) o = std::optional (Key::KEYBOARD_T);
	else if (strcmp (key, "KEYBOARD_Y") == 0) o = std::optional (Key::KEYBOARD_Y);
	else if (strcmp (key, "KEYBOARD_U") == 0) o = std::optional (Key::KEYBOARD_U);
	else if (strcmp (key, "KEYBOARD_I") == 0) o = std::optional (Key::KEYBOARD_I);
	else if (strcmp (key, "KEYBOARD_O") == 0) o = std::optional (Key::KEYBOARD_O);
	else if (strcmp (key, "KEYBOARD_P") == 0) o = std::optional (Key::KEYBOARD_P);
	else if (strcmp (key, "KEYBOARD_LSQB") == 0) o = std::optional (Key::KEYBOARD_LSQB);
	else if (strcmp (key, "KEYBOARD_RSQB") == 0) o = std::optional (Key::KEYBOARD_RSQB);
	else if (strcmp (key, "KEYBOARD_A") == 0) o = std::optional (Key::KEYBOARD_A);
	else if (strcmp (key, "KEYBOARD_S") == 0) o = std::optional (Key::KEYBOARD_S);
	else if (strcmp (key, "KEYBOARD_D") == 0) o = std::optional (Key::KEYBOARD_D);
	else if (strcmp (key, "KEYBOARD_F") == 0) o = std::optional (Key::KEYBOARD_F);
	else if (strcmp (key, "KEYBOARD_G") == 0) o = std::optional (Key::KEYBOARD_G);
	else if (strcmp (key, "KEYBOARD_H") == 0) o = std::optional (Key::KEYBOARD_H);
	else if (strcmp (key, "KEYBOARD_J") == 0) o = std::optional (Key::KEYBOARD_J);
	else if (strcmp (key, "KEYBOARD_K") == 0) o = std::optional (Key::KEYBOARD_K);
	else if (strcmp (key, "KEYBOARD_L") == 0) o = std::optional (Key::KEYBOARD_L);
	else if (strcmp (key, "KEYBOARD_LSHIFT") == 0) o = std::optional (Key::KEYBOARD_LSHIFT);
	else if (strcmp (key, "KEYBOARD_Z") == 0) o = std::optional (Key::KEYBOARD_Z);
	else if (strcmp (key, "KEYBOARD_X") == 0) o = std::optional (Key::KEYBOARD_X);
	else if (strcmp (key, "KEYBOARD_C") == 0) o = std::optional (Key::KEYBOARD_C);
	else if (strcmp (key, "KEYBOARD_V") == 0) o = std::optional (Key::KEYBOARD_V);
	else if (strcmp (key, "KEYBOARD_B") == 0) o = std::optional (Key::KEYBOARD_B);
	else if (strcmp (key, "KEYBOARD_N") == 0) o = std::optional (Key::KEYBOARD_N);
	else if (strcmp (key, "KEYBOARD_M") == 0) o = std::optional (Key::KEYBOARD_M);
	else if (strcmp (key, "KEYBOARD_COMMA") == 0) o = std::optional (Key::KEYBOARD_COMMA);
	else if (strcmp (key, "KEYBOARD_PERIOD") == 0) o = std::optional (Key::KEYBOARD_PERIOD);
	else if (strcmp (key, "KEYBOARD_SLASH") == 0) o = std::optional (Key::KEYBOARD_SLASH);
	else if (strcmp (key, "KEYBOARD_RSHIFT") == 0) o = std::optional (Key::KEYBOARD_RSHIFT);
	else if (strcmp (key, "KEYBOARD_SPACE") == 0) o = std::optional (Key::KEYBOARD_SPACE);
	else if (strcmp (key, "KEYBOARD_ARROW_UP") == 0) o = std::optional (Key::KEYBOARD_ARROW_UP);
	else if (strcmp (key, "KEYBOARD_ARROW_DOWN") == 0) o = std::optional (Key::KEYBOARD_ARROW_DOWN);
	else if (strcmp (key, "KEYBOARD_ARROW_LEFT") == 0) o = std::optional (Key::KEYBOARD_ARROW_LEFT);
	else if (strcmp (key, "KEYBOARD_ARROW_RIGHT") == 0) o = std::optional (Key::KEYBOARD_ARROW_RIGHT);

	free (key);
	return o;
}

extern "C" __declspec (dllexport) void init () {
	auto config = openConfig (std::filesystem::current_path () / "config.toml");
	if (config) {
		auto overrides = toml_array_in (config, "override");
		if (overrides) {
			for (int i = 0; i < 64; i++) {
				auto override = toml_table_at (overrides, i);
				if (!override) break;
				auto buttons = toml_array_in (override, "buttons");
				auto keys    = toml_array_in (override, "keys");
				if (!buttons || !keys) break;
				Override o;
				bool hasButton = false;
				bool hasKey    = false;
				for (int j = 0; j < 8; j++) {
					auto button = toml_string_at (buttons, j);
					if (!button.ok) break;
					if (auto buttonType = buttonStringParse (button.u.s)) {
						o.buttons.push_back (buttonType.value ());
						hasButton = true;
					}
				}
				for (int j = 0;; j++) {
					auto key = toml_string_at (keys, j);
					if (!key.ok) break;
					if (auto keyType = keyStringParse (key.u.s)) {
						o.keys.push_back (keyType.value ());
						hasKey = true;
					}
				}
				if (hasButton && hasKey) {
					o.id = -i + 63;
					Overrides.push_back (o);
				}
			}
		}
	}
	INSTALL_HOOK (RegisterButtonPresses);
	INSTALL_HOOK (RegisterButtonKeys);
}
