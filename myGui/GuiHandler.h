//
// Created by jwhal on 1/13/2025.
//

#ifndef GUIHANDLER_H
#define GUIHANDLER_H

class GuiHandler {
public:
    static void initializeImgui();
    static void plotAudio(bool useLogarithmicScaling);
    static void shutDownImgui();
private:
    static void initializeBackends();
};



#endif //GUIHANDLER_H
