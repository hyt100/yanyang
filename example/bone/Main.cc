#include "App.h"

int main(int argc, char *argv[])
{
    yyCoreInstance.setupWindow("example", 800, 800);
    // yyCoreInstance.setAntiAliasing(true);
    // yyCoreInstance.setOutputSRGB(true);
    yyCoreInstance.init();
    return yyCoreInstance.run(new App());
}
