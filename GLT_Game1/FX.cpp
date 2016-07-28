#include "FX.hpp"

#include "FXCtrl.hpp"

FX::FX() {
	FXCtrl::addFX(this);
}

FX::~FX() {
	finish();
}

void FX::logic() {

}

void FX::draw() {

}

void FX::finish() {
	finished = true;
}