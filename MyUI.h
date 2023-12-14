#pragma once
#include "Dependencies.h"


string FONT_NAME = "Retronoid.otf";
float FONT_TITLE_SIZE = 64;
float FONT_REGULAR_SIZE = 28;
float TEXT_SPACER = 10;


// ========================================
// ========= UI Class Definitions =========
// ========================================

class MyUI
{
protected:
	String string;
	float size;
	Color color;
	Vector2f position;
	Font font;

public:

	MyUI(String string, float size, Color color, Vector2f position) :
		size(size), position(position), color(color), string(string)
	{
		if (!font.loadFromFile(FONT_NAME))
		{
			// error...
			cout << "ERROR LOADING FONT" << endl;
		}
	}

	Text GetText()
	{
		Text text;

		text.setFont(font);
		text.setString(string);
		text.setCharacterSize(size);
		text.setFillColor(color);

		text.setPosition(position);
		FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

		return text;
	}

	void SetText(String newText)
	{
		string = newText;
	}

};

class ScoreBoard : public MyUI
{
protected:
	float minSize;
	float maxSize;
	int playerOneScore;
	int playerTwoScore;
	float timeToShow;
	bool showingScore;
	Vector2f scorePosition;
	Vector2f home;


public:
	ScoreBoard(float size, float maxSize, Color color, Vector2f position, Vector2f scorePosition) :
		MyUI("0 | 0", size, color, position),
		minSize(size),
		maxSize(maxSize),
		playerOneScore(0),
		playerTwoScore(0),
		timeToShow(0),
		showingScore(false),
		scorePosition(scorePosition),
		home(position)
	{
		this->color.a = 50;
	}

	void UpdateScore(bool playerOneScored)
	{
		/*
		if (playerOneScored) playerOneScore++;
		else playerTwoScore++;

		SetText(to_string(playerOneScore) + " | " + to_string(playerTwoScore));

		//display score
		showingScore = true;
		timeToShow = TIME_BETWEEN_ROUNDS; //clean up later
		position = scorePosition;
		size = maxSize;
		color.a = 255;
		*/
	}

	void Update(float deltaTime)
	{
		if (showingScore)
		{
			if (timeToShow < 0)
			{
				showingScore = false;
				position = home;
				size = minSize;
				color.a = 50;
			}
			timeToShow -= deltaTime;
		}
	}


};