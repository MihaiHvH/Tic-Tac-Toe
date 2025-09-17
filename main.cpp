#include "main.hpp"

pScreen screen;
pInterface interface;

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    interface.draw();

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
}

void handleMouseKeys(int button, int state, int x, int y) {
    switch(button) {
        case GLUT_LEFT_BUTTON: {
            screen.leftClick = state;

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
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(screen.initialSize.first, screen.initialSize.second);
    glutCreateWindow (screen.windowName.c_str());

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( 0, screen.initialSize.first, screen.initialSize.second, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    screen.render = render;
    if (interface.AI) interface.handleClick();

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