#pragma once

#include <string>
#include <vector>
#include <GLT\Camera.hpp>
#include <GLT\MeshDrawer.hpp>
#include <GLT\TextMesh.hpp>
#include <GLT\TextBox.hpp>
#include "ScoreboardLoader.hpp"
#include "Timer.hpp"

class ScoreBoard {
public:
	ScoreBoard();
	~ScoreBoard();

	void logic();

	//General draw function
	void draw();

	//Draw big score counter
	void drawScore();
	//Draw score board
	void drawBoard();

	//Load the online scoreboard
	void loadOnline();

	//Set my score
	void setScore(unsigned int score);

private:
	//Entry UI
	//An element that contains data about a single scoreboard entry
	class EntryUI {
	public:  
		//YOffset from center
		static const float verticalOffset;
		//Width of widget
		static const float width;
		//Size of text
		static const float textSize;

		//Index on screen
		int index;
		//Player name
		std::string name;
		//Player score
		int score;
		//Player placing on the leaderboards
		int placing;

		//UI
		glt::TextBox nameBox;
		glt::TextBox scoreBox;
		bool highlighted = false;

		//-------

		EntryUI(int index, std::string name, int placing, int score, bool highlighted);
		EntryUI(int index, ScoreEntry& entry, bool highlighted);

		void loadMeshes();
		void draw(vec2 offset, float alpha);

	private:
		//Used for offsetting in draw method
		vec2 namePosition;
		vec2 scorePosition;
	};

	//Scoreboard result
	//Used to communicate with server
	struct ScoreboardResult {
	public:
		std::vector<ScoreEntry> top;
		std::vector<ScoreEntry> local;

		int myPosition;

		ScoreboardResult() {
			myPosition = -1;
		}

		ScoreboardResult(std::vector<ScoreEntry> top, std::vector<ScoreEntry> local, int myPosition) {
			this->top = top;
			this->local = local;
			this->myPosition = myPosition;
		}
	};

	//Camera
	glt::Camera uiCamera;

	//Background
	glt::MeshDrawer backgroundMesh;

	//UI Elements
	glt::TextBox titleBox;
	glt::TextBox scoreBox;
	glt::TextBox restartBox;
	glt::TextBox errorBox;
	glt::MeshDrawer separatorMesh;

	//... entries
	std::vector<EntryUI*> entryTopList;
	std::vector<EntryUI*> entryLocalList;

	//Timing
	Timer scoreFade = Timer(1.8f);
	Timer boardFade = Timer(1.2f);

	int myScore = 0;

	//--------------------

	void loadScoreboardUI();

	//Online stuff
	static int scoreboardStatus;
	static ScoreboardResult* scoreboardResult;

	static void loadScoreboard(ScoreBoard* board);
};