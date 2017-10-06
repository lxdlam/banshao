#include "showbms.h"
#include "../../Input/gamepad.h"
#include "../../utils.h"
#include <cmath>
#include <filesystem>
using namespace std::chrono;

namespace game
{
	showbms::showbms(std::shared_ptr<Sound> pSound): Scene(pSound)
	{
		if (!font.loadFromFile("resources/sansation.ttf"))
			LOG(WARNING) << "Load font file failed!";
		int idx = 0;
		for (auto& t : text)
		{
			t.setFont(font);
			t.setCharacterSize(24);
			t.setPosition(0, 30.0f * idx++);
			t.setFillColor(sf::Color::White);
		}

		//if (objBms.initWithFile("resources/test.bms"))
		//if (objBms.initWithFile("resources/_goodbounce_yukuri.bms"))
		if (objBms.initWithFile("resources/bms/asdf.bme"))
		{
			LOG(WARNING) << "load bms failed";
			text[0].setString("load failed");
			return;
		}

		auto genre = objBms.getGenre();
		auto artist = objBms.getArtist();
		auto title = objBms.getTitle();
		auto bpm = objBms.getBPM();
		auto notes = objBms.getNoteCount();
		text[0].setString(sf::String::fromUtf32(genre.begin(), genre.end()));
		text[1].setString(sf::String::fromUtf32(artist.begin(), artist.end()));
		text[2].setString(sf::String::fromUtf32(title.begin(), title.end()));
		text[3].setString(std::to_string(bpm));
		text[4].setString(std::to_string(notes));

		// TODO in range check
		judgeTime = defs::judge::judgeTime[objBms.getJudgeRank()];

		loadSprites();

		LOG(DEBUG) << "showbms scene created";
	}

	showbms::~showbms()
	{
		LOG(DEBUG) << "showbms scene destroyed";
	}
	
	//////////////////////////////////////////////////////////
	// Graphic Implements
	//////////////////////////////////////////////////////////

	void showbms::loadSprites()
	{
		auto noteImageIdx = loadImage("resources/note.png");
		loadTexture(noteImageIdx, 0, 0, 32, 4);
		loadTexture(noteImageIdx, 0, 4, 32, 4);
		loadTexture(noteImageIdx, 0, 8, 32, 4);
		notesTextureIdx = { 1, 3, 2, 3, 2, 3, 2, 3, 0, 0 };
		auto noteIdx = createSprite(noteImageIdx, 0, 0, 32, 4);
		vecSprite[noteIdx].setPosition(400, 400);
		createNoteList();
	}

	void showbms::createNoteList()
	{
		for (size_t i = 0; i <= MAXSAMPLEIDX; i++)
		{
			if (!objBms.getWavPath(i).empty())
				soundSystem->loadKeySample(
					objBms.getDirectory()
					+ static_cast<char>(std::experimental::filesystem::path::preferred_separator)
					+ objBms.getWavPath(i)
					, i);
		}

		double basetime = 0;
		double bpm = objBms.getBPM();
		bool bpmfucked = false;
		for (unsigned m = 0; m <= objBms.getMaxMeasure(); m++)
		{

			// notes [] {seg, {lane, sample/val}}
			std::vector<std::pair<rational, std::pair<unsigned, unsigned>>> notes;

			// Visible Notes: 0~19
			for (unsigned i = 0; i < 10; i++)
			{
				auto ch = objBms.getChannel(bmsGetChannelCode::NOTE1, i, m);
				for (const auto& n : ch.notes)
					notes.push_back({ rational(n.first, ch.segments), {i, n.second} });
			}

			// LN: 20~39
			// invisible: 40~59
			// mine: 60~79

			// BGM: 100 ~ 131
			for (unsigned i = 0; i < objBms.getBGMChannelCount(m); i++)
			{
				auto ch = objBms.getChannel(bmsGetChannelCode::BGM, i, m);
				for (const auto& n : ch.notes)
					notes.push_back({ rational(n.first, ch.segments), {100 + i, n.second} });
			}

			// The following patterns must be arranged in specified order

			// BPM Change: FE
			{
				auto ch = objBms.getChannel(bmsGetChannelCode::BPM, 0, m);
				for (const auto& n : ch.notes)
					notes.push_back({ rational(n.first, ch.segments), {0xFE, n.second} });
			}

			// EX BPM: FD
			{
				auto ch = objBms.getChannel(bmsGetChannelCode::EXBPM, 0, m);
				for (const auto& n : ch.notes)
					notes.push_back({ rational(n.first, ch.segments), {0xFD, n.second} });
			}

			// Stop: FF
			{
				auto ch = objBms.getChannel(bmsGetChannelCode::STOP, 0, m);
				for (const auto& n : ch.notes)
					notes.push_back({ rational(n.first, ch.segments), {0xFF, n.second} });
			}

			// Sort by time / lane value
			std::sort(notes.begin(), notes.end());

			// Calculate note times and push to note list
			measureTimeList[m] = basetime;
			rational baseseg(0, 1);
			double len = objBms.getMeasureLength(m) * 2.4e5;
			for (const auto& note : notes)
			{
				auto& seg = note.first;
				auto& lane = note.second.first;
				auto& val = note.second.second;
				double timems = bpmfucked ? INFINITY : basetime + (seg - baseseg) * len / bpm;
				if (lane >= 0 && lane < 20)		// Visible Notes
					noteLists[lane][m].push_back({ seg, timems, val, false });
				else if (lane >= 20 && lane < 40)
				{

				}
				else if (lane >= 40 && lane < 60)
				{

				}
				else if (lane >= 60 && lane < 80)
				{

				}
				else if (lane >= 100 && lane < 132)
					bgmLists[lane - 100][m].push_back({ seg, timems, val, false });

				else if (!bpmfucked) switch (lane)
				{
				case 0xFD:	// ExBPM Change
					basetime += (seg - baseseg) * len / bpm;
					baseseg = seg;
					bpm = objBms.getExBPM(val);
					bpmLists[m].push_back({ seg, timems, bpm });
					if (bpm <= 0) bpmfucked = true;
					break;
				case 0xFE:	// BPM Change
					basetime += (seg - baseseg) * len / bpm;
					baseseg = seg;
					bpm = static_cast<double>(val);
					bpmLists[m].push_back({ seg, timems, bpm });
					if (bpm <= 0) bpmfucked = true;
					break;
				case 0xFF:	// Stop
					double stoplen = objBms.getStop(val);
					if (stoplen <= 0) break;
					stopLists[m].push_back({ seg, timems, stoplen });
					basetime += 1250.0 / bpm * stoplen;		// stoplen / 192
					break;
				}
			}
			basetime += (1.0 - baseseg) * len / bpm;
		}
	}

	void showbms::preDraw()
	{
		if (!started) return;

		// Convert current time(ms) to scale-based position.

		auto relativeTime = duration_cast<milliseconds>(system_clock::now() - startTime).count();
		while (drawMeasure < objBms.getMaxMeasure() && relativeTime >= measureTimeList[drawMeasure + 1])
		{
			drawMeasure++;
			drawBaseseg = rational(0, 1);
			drawBasetime = measureTimeList[drawMeasure];
			itBPM = bpmLists[drawMeasure].begin();
			itStop = stopLists[drawMeasure].begin();
		}

		// BPM
		while (itBPM != bpmLists[drawMeasure].end() && relativeTime >= std::get<1>(*itBPM)
			&& (itStop == stopLists[drawMeasure].end() || std::get<1>(*itBPM) <= std::get<1>(*itStop)))
		{
			drawBaseseg = std::get<0>(*itBPM);
			drawBasetime = std::get<1>(*itBPM);
			drawBPM = std::get<2>(*itBPM);
			itBPM++;
		}

		// Stop
		while (itStop != stopLists[drawMeasure].end() && relativeTime >= std::get<1>(*itStop)
			&& (itBPM == bpmLists[drawMeasure].end() || std::get<1>(*itBPM) > std::get<1>(*itStop)))
		{
			double stopTimeMs = 1250.0 / drawBPM * std::get<2>(*itStop);
			drawBasetime += stopTimeMs;
			drawStopUntil = { std::get<0>(*itStop), std::get<1>(*itStop) + stopTimeMs };
			itStop++;
		}

		if (relativeTime < drawStopUntil.second)
		{
			drawSeg = drawStopUntil.first;
		}
		else
			drawSeg = drawBaseseg + (relativeTime - drawBasetime) / (objBms.getMeasureLength(drawMeasure) * 2.4e5 / drawBPM);

		text[6].setString("#" + std::to_string(drawMeasure));
		text[7].setString(std::to_string(drawSeg * objBms.getMeasureLength(drawMeasure)));
		text[10].setString("Samples: " + std::to_string(soundSystem->getChannelsPlaying()));

	}

	void showbms::createNoteVertices(std::array<sf::VertexArray, 20>& noteVertices) const
	{
		float baseX = 400.0f;
		float baseY = 700.0f;
		double hispeedFactor = baseY / 350.0;
		baseY += drawSeg * objBms.getMeasureLength(drawMeasure) * hispeed * hispeedFactor;
		for (unsigned m = drawMeasure; m <= objBms.getMaxMeasure(); m++)
		{
			auto mlen = objBms.getMeasureLength(m);
			for (unsigned k = 0; k < 10; k++)
			{
				float w = vecTexture[notesTextureIdx[k]].getSize().x;
				float h = vecTexture[notesTextureIdx[k]].getSize().y;
				for (const auto& note : noteLists[k][m])
				{
					const rational& seg = std::get<0>(note);
					const bool& hit = std::get<3>(note);
					if (hit) continue;

					float y = baseY - seg * mlen * hispeed * hispeedFactor;
					if (y < 0) break;
					sf::Vector2f base(baseX + k * w + 5.0f, y);
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0.0f), sf::Vector2f(w, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, h), sf::Vector2f(0.0f, h)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0.0f), sf::Vector2f(w, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, h), sf::Vector2f(0.0f, h)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, h), sf::Vector2f(w, h)));
				}
			}
			baseY -= mlen * hispeed * hispeedFactor;
		}
	}

	void showbms::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::array<sf::VertexArray, 20> noteVertices{};
		for (unsigned k = 0; k < 10; k++)
			noteVertices[k].setPrimitiveType(sf::PrimitiveType::Triangles);
		createNoteVertices(noteVertices);
		for (auto& t: text)
			target.draw(t, states);
		for (unsigned k = 0; k < 10; k++)
		{
			auto s = states;
			s.texture = &vecTexture[notesTextureIdx[k]];
			target.draw(noteVertices[k], s);
		}
	}


	//////////////////////////////////////////////////////////
	// Functional Implements
	//////////////////////////////////////////////////////////

	judgeArea showbms::judgeAreaCheck(double noteTime, double time)
	{
		auto dTime = noteTime - time;

		if (dTime <= judgeTime.BPOOR)
			return judgeArea::BEFORE;
		else if (dTime < judgeTime.BAD)
			return judgeArea::BEFORE_BPOOR;
		else if (dTime < judgeTime.GOOD)
			return judgeArea::BEFORE_BAD;
		else if (dTime < judgeTime.GREAT)
			return judgeArea::BEFORE_GOOD;
		else if (dTime < judgeTime.PERFECT)
			return judgeArea::BEFORE_GREAT;
		else if (-dTime < judgeTime.PERFECT)
			return judgeArea::BEFORE_PERFECT;
		else if (-dTime < judgeTime.GREAT)
			return judgeArea::AFTER_PERFECT;
		else if (-dTime < judgeTime.GOOD)
			return judgeArea::AFTER_GREAT;
		else if (-dTime < judgeTime.BAD)
			return judgeArea::AFTER_GOOD;
		else if (-dTime < judgeTime.POOR)
			return judgeArea::AFTER_BAD;
		else if (-dTime < judgeTime.POOR + 2000)
			return judgeArea::AFTER_POOR;
		else
			return judgeArea::AFTER;
	}

	std::pair<judgeArea, double> showbms::judgeNote(note& note, double time)
	{
		using area = judgeArea;
		auto noteTime = std::get<1>(note);
		auto a = judgeAreaCheck(noteTime, time);
		std::pair<judgeArea, double> ret;
		switch (a)
		{
		case area::BEFORE_BPOOR:	ret = { area::BEFORE_BPOOR, noteTime - time }; break;
		case area::BEFORE_BAD:		ret = { area::BEFORE_BAD, noteTime - time }; break;
		case area::BEFORE_GOOD:		ret = { area::BEFORE_GOOD, noteTime - time }; break;
		case area::BEFORE_GREAT:	ret = { area::BEFORE_GREAT, noteTime - time }; break;
		case area::BEFORE_PERFECT:	ret = { area::BEFORE_PERFECT, noteTime - time }; break;
		case area::AFTER_PERFECT:	ret = { area::AFTER_PERFECT, noteTime - time }; break;
		case area::AFTER_GREAT:		ret = { area::AFTER_GREAT, noteTime - time }; break;
		case area::AFTER_GOOD:		ret = { area::AFTER_GOOD, noteTime - time }; break;
		case area::AFTER_BAD:		ret = { area::AFTER_BAD, noteTime - time }; break;
		case area::AFTER_POOR:		ret = { area::AFTER_POOR, noteTime - time }; break;
		default:					ret = { area::NOTHING, 0 }; break;
		}

		// set hit
		if (ret.first != area::BEFORE_BPOOR && ret.first != area::NOTHING)
			std::get<3>(note) = true;

		return ret;
	}

	void showbms::mainLoop()
	{
		using namespace defs::general;

		if (!started)
		{
			// Hi Speed
			if (drawMeasure > 0)
				if (isFuncKeyPressed(functionalKeys::UP))
					drawMeasure -= 1;
			if (isFuncKeyPressed(functionalKeys::DOWN))
				drawMeasure += 1;
			if (isFuncKeyPressed(functionalKeys::F5))
				start();

		}
		else
		{
			// play sound
			auto relativeTime = duration_cast<milliseconds>(system_clock::now() - startTime).count();

			// bind key sounds
			for (size_t k = 0; k < 10; k++)
			{
				for (auto& note = noteLists[k][drawMeasure].begin(); note != noteLists[k][drawMeasure].end(); note++)
				{
					if (std::get<3>(*note)) continue;
					if (judgeAreaCheck(std::get<1>(*note), relativeTime) != judgeArea::AFTER_KEY)
					{
						keySample[k] = std::get<2>(*note);
						break;
					}
				}
			}

			// keys 1P
			std::vector<size_t> keySamplePlayBuf_1P;
			using keys = gamepadKeys;
			for (int k = keys::S1L; k != keys::K19; k++)
			{
				if (isGamepadKeyPressed(static_cast<keys>(k)))
				{
					keySamplePlayBuf_1P.push_back(keySample[k]);
				}
			}
			soundSystem->playKeySample(keySamplePlayBuf_1P.size(), keySamplePlayBuf_1P.data());
			
			/*
			// auto play ?

			for (size_t k = 0; k < 10; k++)
			{
				for (auto& note = noteLists[k][drawMeasure].begin(); note != noteLists[k][drawMeasure].end(); note++)
				{
					if (std::get<3>(*note)) continue;
					if (std::get<1>(*note) <= relativeTime)
					{
						std::get<3>(*note) = true;
						samplePlayBuf.push_back(std::get<2>(*note));
					}
					else break;
				}
			}
			*/

			// bgms
			std::vector<size_t> samplePlayBuf;
			for (size_t k = 0; k < BGMCHANNELS; k++)
			{
				if (!bgmLists[k][drawMeasure].empty())
					for (auto& note = bgmLists[k][drawMeasure].begin(); note != bgmLists[k][drawMeasure].end(); note++)
					{
						if (std::get<3>(*note)) continue;
						if (std::get<1>(*note) <= relativeTime)
						{
							std::get<3>(*note) = true;
							samplePlayBuf.push_back(std::get<2>(*note));
						}
						else break;
					}
			}

			if (!samplePlayBuf.empty())
				soundSystem->playKeySample(samplePlayBuf.size(), samplePlayBuf.data());
		}

		if (isFuncKeyPressed(functionalKeys::RIGHT))
			hispeed += 25;
		if (hispeed > 25)
			if (isFuncKeyPressed(functionalKeys::LEFT))
				hispeed -= 25;
	}


	void showbms::start()
	{
		started = true;
		startTime = system_clock::now();
		drawMeasure = 0;
		drawSeg = 0;
		drawBPM = objBms.getBPM();
		drawBasetime = 0;
		drawBaseseg = rational(0, 1);
		itBPM = bpmLists[0].begin();
		itStop = stopLists[0].begin();
		drawStopUntil = { rational(0, 1), 0 };
	}
}