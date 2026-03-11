#pragma once

#include <utility>
#include "../../structs.hpp"
#include "../../interface/interface.hpp"

class pGraphics::pMultipleChoiceButon : public pGraphics {
    private:
    int choice = 0;
    /*BUTTON VARS*/

    std::pair<double, double> pos;
    std::pair<double, double> size;
    std::vector<pColor> colors;
    void* font;
    pColor textColor;
    std::string text;
    void(*onClick)(int);

    public:
    pMultipleChoiceButon(std::pair<double, double> pos, std::pair<double, double> size, std::vector<pColor> colors,
                 void* font, pColor textColor, std::string text, void(*onClickFunction)(int));
    ~pMultipleChoiceButon();
    void draw();
    void checkClick();
    void updatePos(std::pair<double, double> pPos);
    void updateSize(std::pair<double, double> pSize);
    void updateText(std::string pText);
};