#pragma once

class FX {
public:
	FX();
	~FX();

	virtual void logic();
	virtual void draw();

	virtual void finish();
	bool isFinished() { return finished; }

private:
	bool finished = false;
};