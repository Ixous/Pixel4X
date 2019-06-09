#pragma once

struct Indices {
    unsigned short x,y;
};

float sigmoid(); //TODO
float cosDecay(float x, float pow);
float expDecay(); //TODO
float cosCurve(float x, float pow);
float bellCurve(); //TODO