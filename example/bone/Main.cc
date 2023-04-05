#include "App.h"

int main(int argc, char *argv[])
{
    yyCoreInstance.setupWindow("example", 1024, 1024);
    // yyCoreInstance.setAntiAliasing(true);
    // yyCoreInstance.setOutputSRGB(true);
    yyCoreInstance.init();
    return yyCoreInstance.run(new App());
}
