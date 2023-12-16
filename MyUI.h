#pragma once

#include "Dependencies.h"

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

	virtual Text GetText()
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

// ========================================
// ======== Score Class Definitions =======
// ========================================

class MyScore : public MyUI
{
private:
	int score;

public:
	MyScore(String string, float size, Color color, Vector2f position) :
		MyUI(string, size, color, position),
		score(0){}

	void IncreaseScore(int points)
	{
		score += points;
		SetText("Score:\n" + to_string(score));
	}

	void ResetScore()
	{
		score = 0;
		SetText("Score:\n0");
	}
};
