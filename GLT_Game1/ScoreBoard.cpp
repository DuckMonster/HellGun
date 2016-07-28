#include "ScoreBoard.hpp"

#include <thread>
#include <GLT\Time.hpp>
#include "AssetBank.hpp"
#include "MathUtil.hpp"

#include "GameState.hpp"
using namespace std;
using namespace glt;

ScoreBoard::ScoreboardResult* ScoreBoard::scoreboardResult = nullptr;

const float ScoreBoard::EntryUI::verticalOffset = 0.5f;
const float ScoreBoard::EntryUI::width = 1.05f;
const float ScoreBoard::EntryUI::textSize = 0.1f;

ScoreBoard::ScoreBoard() : uiCamera(GameState::screenSize) {
	//Create mesh for the fade-background
	float centerWidth = 0.8f;

	float verts[] = {
		-1.f, -1.f, 0.f,
		-centerWidth, -1.f, 0.f,
		-centerWidth, 1.f, 0.f,
		-1.f, 1.f, 0.f,

		-centerWidth, -1.f, 0.f,
		centerWidth, -1.f, 0.f,
		centerWidth, 1.f, 0.f,
		-centerWidth, 1.f, 0.f,

		1.f, -1.f, 0.f,
		centerWidth, -1.f, 0.f,
		centerWidth, 1.f, 0.f,
		1.f, 1.f, 0.f
	};

	float colors[] = {
		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,

		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,

		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f
	};

	backgroundMesh.setMesh(new Mesh());
	backgroundMesh.getMesh()->drawMode = GL_QUADS;
	backgroundMesh.getMesh()->setVertices(verts, sizeof(verts));
	backgroundMesh.getMesh()->setColors(colors, sizeof(colors));
	//

	//Scoreboard separator
	float sep_verts[] = {
		0.f, -0.5f, 0.f,
		0.f, -0.3f, 0.f,
		0.f, 0.3f, 0.f,
		0.f, 0.5f, 0.f
	};

	float sep_colors[] = {
		1.f, 1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f
	};

	separatorMesh.setMesh(new Mesh());
	separatorMesh.getMesh()->setVertices(sep_verts, sizeof(sep_verts));
	separatorMesh.getMesh()->setColors(sep_colors, sizeof(sep_colors));
	separatorMesh.getMesh()->drawMode = GL_LINE_STRIP;
	separatorMesh.scale.y = 0.6f;
	separatorMesh.position.y = 0.3f;

	//

	//Camera stuff
	uiCamera.near = 1.f;
	uiCamera.position.z = 4.f;

	Font* fnt = AssetBank::loadFont("basicFont");

	//Title box
	titleBox.font = fnt;
	titleBox.text = "High Scores";
	titleBox.getTextMesh()->setVAlign(1);
	titleBox.rotation.x = 20.f;

	titleBox.scale = vec3(0.45f);
	titleBox.position.y = 0.55f;
	titleBox.color = vec4(1.f, 0.f, 0.f, 1.f);

	//Restart box
	restartBox.font = fnt;
	restartBox.text = "[LMB] to restart";

	restartBox.scale = vec3(0.2f);
	restartBox.position.y = -0.8f;
	restartBox.rotation.x = -60.f;

	//Score box
	scoreBox.font = fnt;
	scoreBox.scale = vec3(0.9f);

	//Error box
	errorBox.font = fnt;
	errorBox.text = "Error retreiving scoreboard";
	errorBox.color = vec4(1.f, 0.f, 0.f, 1.f);
	errorBox.position.y = 0.6f;
	errorBox.scale = vec3(0.2f);
}

ScoreBoard::~ScoreBoard() {
	for (int i = 0; i < entryTopList.size(); i++)
		delete entryTopList[i];
	for (int i = 0; i < entryLocalList.size(); i++)
		delete entryLocalList[i];

	delete separatorMesh.getMesh();
}

//Set my score
void ScoreBoard::setScore(unsigned int score) {
	myScore = score;
	scoreBox.text = to_string(myScore);
}

void ScoreBoard::logic() {

}

void ScoreBoard::draw() {
	uiCamera.setActive();

	backgroundMesh.color = vec4(0.f, 0.f, 0.f, 0.6f * scoreFade.value);
	backgroundMesh.scale.x = 0.4f * linToExp(scoreFade.value) + 1.4f * linToExp(boardFade.value);
	backgroundMesh.draw();

	drawScore();

	if (scoreFade.isDone())
		drawBoard();
}

void ScoreBoard::drawScore() {
	scoreFade.update();

	scoreBox.color.a = scoreFade.value * 4.f;
	scoreBox.rotation.y = 90.f - 90.f * linToExp(scoreFade.value) + 20.f * sin(Time::total() * 0.2f) * boardFade.value;
	scoreBox.position.y = (-0.1f + 0.1f * linToExp(scoreFade.value)) - 0.5f * linToExp(boardFade.value) + 0.05f * sin(Time::total() * 0.5f) * boardFade.value;
	scoreBox.draw();
}

void ScoreBoard::drawBoard() {
	boardFade.update();

	titleBox.color.a = boardFade.value;

	if (scoreboardStatus == 1) {
		if (scoreboardResult != nullptr)
			loadScoreboardUI();

		float yOffset = 0.005f * sin(Time::total() * 0.5f);

		for (int i = 0; i < entryTopList.size(); i++)
			entryTopList[i]->draw(vec2(-EntryUI::width / 2.f - 0.06f, yOffset),  boardFade.value);

		for (int i = 0; i < entryLocalList.size(); i++)
			entryLocalList[i]->draw(vec2(EntryUI::width / 2.f + 0.06f, yOffset), boardFade.value);

		separatorMesh.color.a = boardFade.value;
		separatorMesh.draw();
	}
	else if (scoreboardStatus == -1)
		errorBox.draw();

	titleBox.rotation.y = 5.f * sin(Time::total() * 0.243f);
	titleBox.position.y = 0.55f + 0.005f * sin(Time::total() * 0.6213f);
	titleBox.draw();

	restartBox.scale = vec3(0.3f + 0.05f * sin(Time::total()));
	restartBox.color.a = 0.8f + 0.2f * sin(Time::total());
	restartBox.draw();
}

//ENTRY
ScoreBoard::EntryUI::EntryUI(int index, string name, int placing, int score, bool highlighted) {
	this->index = index;
	this->name = name;
	this->placing = placing;
	this->score = score;
	this->highlighted = highlighted;

	loadMeshes();
}

ScoreBoard::EntryUI::EntryUI(int index, ScoreEntry& entry, bool highlighted) {
	this->index = index;
	name = entry.name;
	placing = entry.position;
	score = entry.score;
	this->highlighted = highlighted;

	loadMeshes();
}

void ScoreBoard::EntryUI::loadMeshes() {
	Font* fnt = AssetBank::loadFont("scoreboardFont");

	//Set base position
	namePosition = vec2(-width * 0.5f, verticalOffset - textSize * 0.75f * index);
	scorePosition = vec2(width * 0.5f, verticalOffset - textSize * 0.75f * index);

	//Name box
	nameBox.scale = vec3(textSize);
	nameBox.font = fnt;
	nameBox.text = to_string(placing + 1) + ". " + name;
	nameBox.getTextMesh()->setHAlign(-1);
	nameBox.getTextMesh()->setSpacing(0.05f);

	//Score box
	scoreBox.position.x = width * 0.5f;
	scoreBox.position.y = verticalOffset + textSize * index;
	scoreBox.scale = vec3(textSize),
	scoreBox.font = fnt;
	scoreBox.text = to_string(score);
	scoreBox.getTextMesh()->setHAlign(1);
	scoreBox.getTextMesh()->setSpacing(0.05f);

	if (highlighted) {
		nameBox.color = vec4(1.f, 1.f, 0.f, 1.f);
		scoreBox.color = vec4(1.f, 1.f, 0.f, 1.f);
	}
}

void ScoreBoard::EntryUI::draw(vec2 offset, float alpha) {
	nameBox.position = vec3(namePosition + offset, 0.f);
	nameBox.color.a = alpha;
	nameBox.draw();

	scoreBox.position = vec3(scorePosition + offset, 0.f);
	scoreBox.color.a = alpha;
	scoreBox.draw();
}

//ONLINE FUNCTIONALITY
int ScoreBoard::scoreboardStatus = 0;

void ScoreBoard::loadOnline() {
	scoreboardStatus = 0;
	std::thread(ScoreBoard::loadScoreboard, this).detach();
}

//Load scoreboard from server
void ScoreBoard::loadScoreboard(ScoreBoard* board) {
	try {
		Connection server("178.62.172.189", "7777");

#ifndef IGNORE_SCORE

		int myPos = server.addEntry(GameState::playerName, board->myScore);

		vector<ScoreEntry> top = server.requestEntry(0, 5);
		int localOffset = std::max(myPos - 2, 0);
		vector<ScoreEntry> local = server.requestEntry(localOffset, 5);
		
		scoreboardResult = new ScoreboardResult(top, local, myPos);
		scoreboardStatus = 1;

#endif
#ifdef IGNORE_SCORE

		vector<ScoreEntry> top = server.requestEntry(0, 5);
		vector<ScoreEntry> local = server.requestEntry(5, 5);

		scoreboardResult = new ScoreboardResult(top, local, 0);
		scoreboardStatus = 1;

#endif

		server.disconnect();
	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
		scoreboardStatus = -1;
	}
}

void ScoreBoard::loadScoreboardUI() {
	ScoreboardResult& result = *scoreboardResult;

	for (int i = 0; i < result.top.size(); i++) {
		ScoreEntry& entry = result.top[i];
		entryTopList.push_back(new EntryUI(i, entry, entry.position == result.myPosition));
	}

	for (int i = 0; i < result.local.size(); i++) {
		ScoreEntry& entry = result.local[i];
		entryLocalList.push_back(new EntryUI(i, entry, entry.position == result.myPosition));
	}

	delete scoreboardResult;
	scoreboardResult = nullptr;
}