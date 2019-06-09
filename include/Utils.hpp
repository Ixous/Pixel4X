#pragma once

struct Indices {
    unsigned short x,y;
};

float sigmoid(); //TODO
float cosDecay(float x, float pow=1);
float expDecay(); //TODO
float cosCurve(float x, float pow=1);
float bellCurve(); //TODO