#include "keyconfig.h"
#include <SFML/Window/Keyboard.hpp>
#include "../defs.h"
#include "../utils.h"
#include <array>
#include <fstream>

namespace game {
	namespace config

	{
		using namespace defs::config;

		void keyconfig::clear() noexcept
		{
			std::array<std::array<unsigned, KEYSLOTS>, keys::GAMEPAD_KEY_COUNT> blank{};
			_json[k_5keys] = json(blank);
			_json[k_7keys] = json(blank);
			_json[k_9keys] = json(blank);
		}

		void keyconfig::setDefaults() noexcept
		{
			clear();

			// QWERTY
			// How about Dvorak / QZERTY / others..?

			// 5Keys default
			{
				bindKey(5, keys::S1L, 0, sf::Keyboard::LShift);
				bindKey(5, keys::S1R, 0, sf::Keyboard::LControl);
				bindKey(5, keys::K11, 0, sf::Keyboard::Z);
				bindKey(5, keys::K12, 0, sf::Keyboard::S);
				bindKey(5, keys::K13, 0, sf::Keyboard::X);
				bindKey(5, keys::K14, 0, sf::Keyboard::D);
				bindKey(5, keys::K15, 0, sf::Keyboard::C);
				bindKey(5, keys::K1START, 0, sf::Keyboard::Q);
				bindKey(5, keys::K1SELECT, 0, sf::Keyboard::W);

				bindKey(5, keys::S2L, 0, sf::Keyboard::RShift);
				bindKey(5, keys::S2R, 0, sf::Keyboard::RControl);
				bindKey(5, keys::K21, 0, sf::Keyboard::Comma);
				bindKey(5, keys::K22, 0, sf::Keyboard::L);
				bindKey(5, keys::K23, 0, sf::Keyboard::Period);
				bindKey(5, keys::K24, 0, sf::Keyboard::SemiColon);
				bindKey(5, keys::K25, 0, sf::Keyboard::Slash);
				bindKey(5, keys::K2START, 0, sf::Keyboard::P);
				bindKey(5, keys::K2SELECT, 0, sf::Keyboard::RBracket);
			}

			// 7Keys default
			{
				bindKey(7, keys::S1L, 0, sf::Keyboard::LShift);
				bindKey(7, keys::S1R, 0, sf::Keyboard::LControl);
				bindKey(7, keys::S1R, 1, sf::Keyboard::Space);
				bindKey(7, keys::K11, 0, sf::Keyboard::Z);
				bindKey(7, keys::K12, 0, sf::Keyboard::S);
				bindKey(7, keys::K13, 0, sf::Keyboard::X);
				bindKey(7, keys::K14, 0, sf::Keyboard::D);
				bindKey(7, keys::K15, 0, sf::Keyboard::C);
				bindKey(7, keys::K16, 0, sf::Keyboard::F);
				bindKey(7, keys::K17, 0, sf::Keyboard::V);
				bindKey(7, keys::K1START, 0, sf::Keyboard::Q);
				bindKey(7, keys::K1SELECT, 0, sf::Keyboard::W);

				bindKey(7, keys::S2L, 0, sf::Keyboard::RShift);
				bindKey(7, keys::S2R, 0, sf::Keyboard::RControl);
				bindKey(7, keys::K21, 0, sf::Keyboard::M);
				bindKey(7, keys::K22, 0, sf::Keyboard::K);
				bindKey(7, keys::K23, 0, sf::Keyboard::Comma);
				bindKey(7, keys::K24, 0, sf::Keyboard::L);
				bindKey(7, keys::K25, 0, sf::Keyboard::Period);
				bindKey(7, keys::K26, 0, sf::Keyboard::SemiColon);
				bindKey(7, keys::K27, 0, sf::Keyboard::Slash);
				bindKey(7, keys::K2START, 0, sf::Keyboard::P);
				bindKey(7, keys::K2SELECT, 0, sf::Keyboard::RBracket);
			}

			// 9Keys default
			{
				bindKey(9, keys::S1L, 0, sf::Keyboard::LShift);
				bindKey(9, keys::S1R, 0, sf::Keyboard::LControl);
				bindKey(9, keys::K11, 0, sf::Keyboard::Z);
				bindKey(9, keys::K12, 0, sf::Keyboard::S);
				bindKey(9, keys::K13, 0, sf::Keyboard::X);
				bindKey(9, keys::K14, 0, sf::Keyboard::D);
				bindKey(9, keys::K15, 0, sf::Keyboard::C);
				bindKey(9, keys::K16, 0, sf::Keyboard::F);
				bindKey(9, keys::K17, 0, sf::Keyboard::V);
				bindKey(9, keys::K18, 0, sf::Keyboard::G);
				bindKey(9, keys::K19, 0, sf::Keyboard::B);
				bindKey(9, keys::K1START, 0, sf::Keyboard::Q);
				bindKey(9, keys::K1SELECT, 0, sf::Keyboard::W);

				bindKey(9, keys::S2L, 0, sf::Keyboard::RShift);
				bindKey(9, keys::S2R, 0, sf::Keyboard::RControl);
				bindKey(9, keys::K21, 0, sf::Keyboard::N);
				bindKey(9, keys::K22, 0, sf::Keyboard::J);
				bindKey(9, keys::K23, 0, sf::Keyboard::M);
				bindKey(9, keys::K24, 0, sf::Keyboard::K);
				bindKey(9, keys::K25, 0, sf::Keyboard::Comma);
				bindKey(9, keys::K26, 0, sf::Keyboard::L);
				bindKey(9, keys::K27, 0, sf::Keyboard::Period);
				bindKey(9, keys::K28, 0, sf::Keyboard::SemiColon);
				bindKey(9, keys::K29, 0, sf::Keyboard::Slash);
				bindKey(9, keys::K2START, 0, sf::Keyboard::P);
				bindKey(9, keys::K2SELECT, 0, sf::Keyboard::RBracket);
			}
		}

		int keyconfig::loadFile(const std::string jsonPath) noexcept
		{
			setDefaults();
			std::ifstream inFile(jsonPath);
			if (inFile.fail())
			{
				LOG(WARNING) << "Load keyconfig file failed: " << jsonPath << ", using default.";
				saveFile(jsonPath);
				return -1;
			}

			json tmp;
			inFile >> tmp;
			int c = copyValues(tmp);
			checkValues();
			return c;
		}

		int keyconfig::saveFile(const std::string jsonPath) noexcept
		{
			std::ofstream outFile(jsonPath);
			if (outFile.fail())
			{
				LOG(ERROR) << "Save to keyconfig file failed: " << jsonPath;
				return -1;
			}

			outFile << _json.dump(4) << std::endl;
			return 0;
		}

		bool keyconfig::checkUnsigned(const json& j, const std::string& key, unsigned target, unsigned slot)
		{
			if (j[key][target][slot].is_number_unsigned())
			{
				_json[key][target][slot] = j[key][target][slot];
				return false;
			}
			LOG(WARNING) << "Value Type(Unsigned) Check Error ( " << key << " : " << j[key] << " )";
			return true;
		}

		int keyconfig::copyValues(const json &j) noexcept
		{
			int c = 0;

			for (unsigned k = 0; k < keys::GAMEPAD_KEY_COUNT; k++)
				for (unsigned slot = 0; slot < KEYSLOTS; slot++)
					c += checkUnsigned(j, k_5keys, k, slot);

			for (unsigned k = 0; k < keys::GAMEPAD_KEY_COUNT; k++)
				for (unsigned slot = 0; slot < KEYSLOTS; slot++)
					c += checkUnsigned(j, k_7keys, k, slot);

			for (unsigned k = 0; k < keys::GAMEPAD_KEY_COUNT; k++)
				for (unsigned slot = 0; slot < KEYSLOTS; slot++)
					c += checkUnsigned(j, k_9keys, k, slot);

			return c;
		}

		void keyconfig::_bindKey(unsigned K, keys target, unsigned slot, unsigned device, unsigned key)
		{
			//gamepadKeyBindings[target][slot] = {device, key};
			std::string k;
			switch (K)
			{
			case 5: k = k_5keys; break;
			case 7: k = k_7keys; break;
			case 9: k = k_9keys; break;
			}
			if (!k.empty())
				_json[k][target][slot] = key;
		}

		void keyconfig::bindKey(unsigned K, keys target, unsigned slot, sf::Keyboard::Key key)
		{
			if (key == sf::Keyboard::Key::Unknown) return;
			_bindKey(K, target, slot, 0, static_cast<unsigned>(key));
		}

		void keyconfig::bindKey(unsigned K, keys target, unsigned slot, unsigned joyNo, unsigned button)
		{
			_bindKey(K, target, slot, (joyNo + 1) * 10000, static_cast<int>(button));
		}

		void keyconfig::bindKey(unsigned K, keys target, unsigned slot, unsigned joyNo, sf::Joystick::Axis axis, int direction)
		{
			bindKey(K, target, slot, joyNo, 1000 + (direction > 0 ? 100 : 0) + axis);
		}

		std::array<std::pair<int, int>, KEYSLOTS> keyconfig::getBindings(unsigned K, keys button) const
		{
			std::string k;
			switch (K)
			{
			case 5: k = k_5keys; break;
			case 7: k = k_7keys; break;
			case 9: k = k_9keys; break;
			}
			if (k.empty()) return {};

			std::array<std::pair<int, int>, KEYSLOTS> res{};
			for (size_t i = 0; i < KEYSLOTS; i++)
			{
				unsigned val = _json[k][button][i];
				if (val == 0) continue;
				int device = (val / 10000) - 1;
				int code = val % 10000;
				res[i] = { device, code };
			}

			return res;
		}

		int keyconfig::checkValues() noexcept
		{
			// TODO
			return 0;
		}
	}
}