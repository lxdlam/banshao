#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../defs.h"
#include "../sound/sound.h"
#include "../skin/skin.h"
#include <bitset>
#include <future>

namespace game
{
	using namespace defs::key;

	// Parent class of scenes, defines how an object being stored and drawn.
	// Every classes of scenes should inherit this class.
	// Must import pointer of sound system when constructing.
	// vecImage[0] & vecTexture[0] stores error texture.
	class Scene: public sf::Drawable
	{
	protected:
		//std::vector<sf::Image> vecImage;
		//std::vector<sf::Texture> vecTexture;
		//std::vector<sf::Sprite> vecSprite;
		skinClass skin;

	public:
		Scene() = delete;
		Scene(std::shared_ptr<Sound>);
		virtual ~Scene();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/*
	private:
		// Virtual function that loads textures into sprites.
		// Called at the end of constructor.
		// Should check index to prevent overflow.
		virtual void loadSprites();
	protected:
		size_t loadImage(std::string path);
		size_t loadTexture(size_t imageIdx);
		size_t loadTexture(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h);
		size_t createSprite(size_t textureidx);
		size_t createSprite(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h);
		*/

	private:
		bool active = false;
		bool running = false;
		bool saveCPU = true;
		int fps = 0;
		std::future<void> inputTaskFuture;
		void input_thread_func();
		std::bitset<FUNC_KEY_COUNT> prev_functionalInput = 0;
		std::bitset<GAMEPAD_KEY_COUNT> prev_gamepadInput = 0;
		std::bitset<FUNC_KEY_COUNT> functionalInput = 0;
		std::bitset<GAMEPAD_KEY_COUNT> gamepadInput = 0;
		
	public:
		void run();
		void setHighResponsive(bool);
		void setActive(bool);
		bool Scene::isFuncKeyPressed(functionalKeys k) const;
		bool Scene::isGamepadKeyPressed(gamepadKeys k) const;
		bool Scene::isFuncKeyReleased(functionalKeys k) const;
		bool Scene::isGamepadKeyReleased(gamepadKeys k) const;
		bool Scene::isFuncKeyHolding(functionalKeys k) const;
		bool Scene::isGamepadKeyHolding(gamepadKeys k) const;
		const std::bitset<FUNC_KEY_COUNT>& getFunctionalInput() const;
		const std::bitset<GAMEPAD_KEY_COUNT>& getGamepadInput() const;

	public:
		struct setDst { dst_option dstIdx; bool val; };
	protected:
		std::array<std::vector<timer>, GAMEPAD_KEY_COUNT> gamepadTimerPressMap
		{ {
			{timer::KEY_ON_SCR_1P},	// 1P Scratch L
			{timer::KEY_ON_SCR_1P}, // 1P Scratch R
			{timer::PANEL1_ON, timer::KEY_ON_1_1P},
			{timer::PANEL2_ON, timer::KEY_ON_2_1P},
			{timer::PANEL3_ON, timer::KEY_ON_3_1P},
			{timer::PANEL4_ON, timer::KEY_ON_4_1P},
			{timer::PANEL5_ON, timer::KEY_ON_5_1P},
			{timer::PANEL6_ON, timer::KEY_ON_6_1P},
			{timer::PANEL7_ON, timer::KEY_ON_7_1P},
			{timer::PANEL8_ON, timer::KEY_ON_8_1P},
			{timer::PANEL9_ON, timer::KEY_ON_9_1P},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{}, // 2P Scratch R
			{timer::PANEL1_ON, timer::KEY_ON_1_2P},
			{timer::PANEL2_ON, timer::KEY_ON_2_2P},
			{timer::PANEL3_ON, timer::KEY_ON_3_2P},
			{timer::PANEL4_ON, timer::KEY_ON_4_2P},
			{timer::PANEL5_ON, timer::KEY_ON_5_2P},
			{timer::PANEL6_ON, timer::KEY_ON_6_2P},
			{timer::PANEL7_ON, timer::KEY_ON_7_2P},
			{timer::PANEL8_ON, timer::KEY_ON_8_2P},
			{timer::PANEL9_ON, timer::KEY_ON_9_2P},
			{timer::KEY_ON_SCR_2P},	// 2P Start
			{timer::KEY_ON_SCR_2P},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		std::array<std::vector<setDst>, GAMEPAD_KEY_COUNT> gamepadDstOptPressMap
		{ {
			{},	// 1P Scratch L
			{}, // 1P Scratch R
			{{dst_option::PANEL1_ON, true}},
			{{dst_option::PANEL2_ON, true}},
			{{dst_option::PANEL3_ON, true}},
			{{dst_option::PANEL4_ON, true}},
			{{dst_option::PANEL5_ON, true}},
			{{dst_option::PANEL6_ON, true}},
			{{dst_option::PANEL7_ON, true}},
			{{dst_option::PANEL8_ON, true}},
			{{dst_option::PANEL9_ON, true}},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{},	// 2P Scratch R
			{{dst_option::PANEL1_ON, true}},
			{{dst_option::PANEL2_ON, true}},
			{{dst_option::PANEL3_ON, true}},
			{{dst_option::PANEL4_ON, true}},
			{{dst_option::PANEL5_ON, true}},
			{{dst_option::PANEL6_ON, true}},
			{{dst_option::PANEL7_ON, true}},
			{{dst_option::PANEL8_ON, true}},
			{{dst_option::PANEL9_ON, true}},
			{},	// 2P Start
			{},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		std::array<std::vector<timer>, GAMEPAD_KEY_COUNT> gamepadTimerReleaseMap
		{ {
			{timer::KEY_OFF_SCR_1P},	// 1P Scratch L
			{timer::KEY_OFF_SCR_1P}, // 1P Scratch R
			{timer::PANEL1_OFF, timer::KEY_OFF_1_1P},
			{timer::PANEL2_OFF, timer::KEY_OFF_2_1P},
			{timer::PANEL3_OFF, timer::KEY_OFF_3_1P},
			{timer::PANEL4_OFF, timer::KEY_OFF_4_1P},
			{timer::PANEL5_OFF, timer::KEY_OFF_5_1P},
			{timer::PANEL6_OFF, timer::KEY_OFF_6_1P},
			{timer::PANEL7_OFF, timer::KEY_OFF_7_1P},
			{timer::PANEL8_OFF, timer::KEY_OFF_8_1P},
			{timer::PANEL9_OFF, timer::KEY_OFF_9_1P},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{}, // 2P Scratch R
			{timer::PANEL1_OFF, timer::KEY_OFF_1_2P},
			{timer::PANEL2_OFF, timer::KEY_OFF_2_2P},
			{timer::PANEL3_OFF, timer::KEY_OFF_3_2P},
			{timer::PANEL4_OFF, timer::KEY_OFF_4_2P},
			{timer::PANEL5_OFF, timer::KEY_OFF_5_2P},
			{timer::PANEL6_OFF, timer::KEY_OFF_6_2P},
			{timer::PANEL7_OFF, timer::KEY_OFF_7_2P},
			{timer::PANEL8_OFF, timer::KEY_OFF_8_2P},
			{timer::PANEL9_OFF, timer::KEY_OFF_9_2P},
			{timer::KEY_OFF_SCR_2P},	// 2P Start
			{timer::KEY_OFF_SCR_2P},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		std::array<std::vector<setDst>, GAMEPAD_KEY_COUNT> gamepadDstOptReleaseMap
		{ {
			{},	// 1P Scratch L
			{}, // 1P Scratch R
			{{dst_option::PANEL1_ON, false}},
			{{dst_option::PANEL2_ON, false}},
			{{dst_option::PANEL3_ON, false}},
			{{dst_option::PANEL4_ON, false}},
			{{dst_option::PANEL5_ON, false}},
			{{dst_option::PANEL6_ON, false}},
			{{dst_option::PANEL7_ON, false}},
			{{dst_option::PANEL8_ON, false}},
			{{dst_option::PANEL9_ON, false}},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{},	// 2P Scratch R
			{{dst_option::PANEL1_ON, false}},
			{{dst_option::PANEL2_ON, false}},
			{{dst_option::PANEL3_ON, false}},
			{{dst_option::PANEL4_ON, false}},
			{{dst_option::PANEL5_ON, false}},
			{{dst_option::PANEL6_ON, false}},
			{{dst_option::PANEL7_ON, false}},
			{{dst_option::PANEL8_ON, false}},
			{{dst_option::PANEL9_ON, false}},
			{},	// 2P Start
			{},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };

	protected:
		bool startedInput = false;
		std::shared_ptr<Sound> soundSystem;
		void checkKeys(unsigned rTime);
		virtual void mainLoop();
		void close();
	};
}
