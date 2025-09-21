#include "main.hpp"

pScreen screen;
pInterface interface;

void render();

pGraphics::pButon AIBtn({ 50, 50 }, { 100, 20 }, interface.AI ? interface.graphics.blue : interface.graphics.red, interface.AI ? interface.graphics.red : interface.graphics.blue, GLUT_BITMAP_HELVETICA_18, interface.graphics.black, interface.AI ? "AI: ON" : "AI: OFF", [](bool state) {
    interface.AI = !interface.AI;
    AIBtn.updateText(interface.AI ? "AI: ON" : "AI: OFF");
    render();
});

pGraphics::pButon AIRandomStartBtn({ 50, 50 }, { 100, 20 }, interface.AIRandomStart ? interface.graphics.blue : interface.graphics.red, interface.AIRandomStart ? interface.graphics.red : interface.graphics.blue, GLUT_BITMAP_HELVETICA_18, interface.graphics.black, interface.AIRandomStart ? "AI Random Start: ON" : "AI Random Start: OFF", [](bool state) {
    interface.AIRandomStart = !interface.AIRandomStart;
    AIRandomStartBtn.updateText(interface.AIRandomStart ? "AI Random Start: ON" : "AI Random Start: OFF");
    render();
});

pGraphics::pButon AIXor0Btn({ 50, 50 }, { 100, 20 }, interface.AIXor0 ? interface.graphics.cyan : interface.graphics.purple, interface.AIXor0 ? interface.graphics.purple : interface.graphics.cyan, GLUT_BITMAP_HELVETICA_18, interface.graphics.black, interface.AIXor0 ? "AI: 0" : "AI: X", [](bool state) {
    interface.AIXor0 = !interface.AIXor0;
    interface.turn = interface.AIXor0;
    AIXor0Btn.updateText(interface.AIXor0 ? "AI: 0" : "AI: X");
    render();
});

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    interface.draw();
    if (interface.settings) {
        std::pair<double, double> size = { screen.size.first / 3.25f, screen.size.second / 10.f };
        
        AIBtn.updatePos({ screen.size.first / 2.f - size.first / 2.f, (screen.size.second - size.second) / 2.f - 2 * size.second / 1.5f });
        AIBtn.updateSize(size);
        AIBtn.draw();

        AIRandomStartBtn.updatePos({ (screen.size.first - size.first) / 2.f, (screen.size.second - size.second) / 2.f });
        AIRandomStartBtn.updateSize(size);
        AIRandomStartBtn.draw();

        AIXor0Btn.updatePos({ (screen.size.first - size.first) / 2.f, (screen.size.second - size.second) / 2.f + size.second + (size.second - size.second / 1.5f) });
        AIXor0Btn.updateSize(size);
        AIXor0Btn.draw();

        std::pair<int, int> tSize = interface.graphics.getTextSize("Press ESC to start", GLUT_BITMAP_HELVETICA_18);
        
        interface.graphics.drawText({ (screen.size.first - tSize.first) / 2.f, (screen.size.second - size.second) / 2.f - 2 * size.second / 1.5 - tSize.second / 2.f }, GLUT_BITMAP_HELVETICA_18, "Press ESC to start", interface.graphics.black);
    }
    glutSwapBuffers();
}

void resize(GLint newWidth, GLint newHeight) {
    if (newWidth >= 8 && newHeight >= 8) {
        screen.size = { newWidth, newHeight };
        
        glViewport( 0, 0, newWidth, newHeight );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, GLdouble (newWidth), GLdouble (newHeight), 0);
        glutPostRedisplay();
    }
}

void processSpecialInput(int key, int x, int y) {
}

void processInput(unsigned char key, int x, int y) {
    if ((int)key == 27) interface.drawSettings();
}

void handleMouseKeys(int button, int state, int x, int y) {
    switch(button) {
        case GLUT_LEFT_BUTTON: {
            screen.leftClick = state;

            if (interface.settings) {
                AIBtn.checkClick();
                AIRandomStartBtn.checkClick();
                AIXor0Btn.checkClick();
            }

            if (state != GLUT_DOWN)
                break;

            interface.handleClick();

            break;
        }
        case GLUT_RIGHT_BUTTON: {
            screen.rightClick = state;

            if (state != GLUT_DOWN)
                break;

            break;
        }
    }
}

void handleMouseMovement(int x, int y) {
    screen.mousePointer = { x, y };
}

void handleMouseDrag(int x, int y) {
    screen.mousePointer = { x, y };
    if (screen.leftClick != screen.leftClickDrag)
        screen.leftClickDrag = screen.leftClick;
}

void handleIdle() {
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(screen.initialSize.first, screen.initialSize.second);
    glutCreateWindow (screen.windowName.c_str());

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( 0, screen.initialSize.first, screen.initialSize.second, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    screen.render = render;
    interface.drawSettings();

    glutDisplayFunc(render);
    glutKeyboardFunc(processInput);
    glutSpecialFunc(processSpecialInput);
    glutReshapeFunc(resize);
    glutMouseFunc(handleMouseKeys);
    glutPassiveMotionFunc(handleMouseMovement);
    glutMotionFunc(handleMouseDrag);
    glutIdleFunc(handleIdle);

    glutMainLoop();

    return 0;
}