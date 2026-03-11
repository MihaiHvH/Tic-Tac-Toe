#include "../../main.hpp"
#include "multipleChoiceButon.hpp"

#ifdef _WIN32
    #undef interface
#endif

pGraphics::pMultipleChoiceButon::~pMultipleChoiceButon() {

}

pGraphics::pMultipleChoiceButon::pMultipleChoiceButon(std::pair<double, double> pPos, std::pair<double, double> pSize, std::vector<pColor> pColors,
                 void* pFont, pColor pTextColor, std::string pText, void(*onClickFunction)(int)) {
    pos = pPos;
    size = pSize;
    colors = pColors;
    font = pFont;
    textColor = pTextColor;
    text = pText;
    onClick = onClickFunction;
}

void pGraphics::pMultipleChoiceButon::checkClick() {
    if (this->mouseInRegion(pos, size) && screen.leftClick) {
        if (++choice >= colors.size())
            choice = 0;
        onClick(choice);
    }
}

void pGraphics::pMultipleChoiceButon::draw() {
    pColor color = colors.at(choice);
    std::pair<int, int> textSize = this->getTextSize(text.c_str(), font);
    this->drawRect(pos, size, color);
    this->drawText({ pos.first + ((size.first - textSize.first) / 2), pos.second + (size.second / 2) + textSize.second / 2 }, font, text.c_str(), textColor);   
}

void pGraphics::pMultipleChoiceButon::updatePos(std::pair<double, double> pPos) {
    pos = pPos;
}

void pGraphics::pMultipleChoiceButon::updateSize(std::pair<double, double> pSize) {
    size = pSize;
}

void pGraphics::pMultipleChoiceButon::updateText(std::string pText) {
    text = pText;
}